#include<iostream>
using namespace std;

int final=0;
int __inline__ atomicCAS( volatile int *ptr, int cmp, int newv)
{
    unsigned char ret;
    __asm__ __volatile__ (
    " lock\n"
    " cmpxchgl %2,%1\n"
    " sete %0\n"
    : "=q" (ret), "=m" (*ptr)
    : "r" (newv), "m" (*ptr), "a" (cmp)
    : "memory");

    return (int) ret;
}


void *myFunc(void * a )

{
    long master= (long)a;
    uint32_t masterDeserialize=uint32_t(master);
  
    unsigned int tid = pthread_self();
    // __asm__ __volatile__ ("" ::: "memory");
    int y=0;
    for(int i=0;i<10000;i++){
        y+=i;
    }
    int prev=final;
    uint32_t retMaster= __sync_bool_compare_and_swap (&final, prev, prev+master);//__sync_val_compare_and_swap(&final, final, final+master); 
    // while(prev==retMaster){
        // retMaster =  __sync_val_compare_and_swap(&final, final, prev+master); 
    // }
    if(retMaster==0){
        cout<<"Race condition occured and wrong value updated while adding " <<master<<endl;
    }
    while(retMaster==0){
      
        prev=final;
        retMaster= __sync_bool_compare_and_swap (&final, prev, prev+master);
    }
 




    // final+=master;
    // cout<<final<<"**********"<<endl;
    // cout << "After cas, threadid = " << tid << " retMaster = " << retMaster << " MasterDeserialize = " << masterDeserialize << " masterAddress = " << &masterDeserialize << std::endl;
    // if(retMaster != 0) // not master deserializer.
    // {
    //    cout<< "getConfigurationRowField, threadId: " << tid << " NOT master.  retMaster = " << retMaster << std::endl;

    // }
    // else
    // {
    //     // final+=master;
    //     // cout << "getConfigurationRowField, threadId: " << tid << " MASTER. retMaster = " << retMaster << std::endl;
    //     cout << final<<"***************";


    //     masterDeserialize = 0;
    // }
  
}
int main(){
    int k=1000;
    pthread_t threads[1000];
    uint32_t out=1000;
    pthread_t ptid;
    for(int i=1;i<k;i++){
        pthread_create(&threads[i], NULL, myFunc,(void *)i );

    }
    for(int i=0;i<k;i++){

        pthread_join(threads[i], NULL);
    }
    cout<<final<<"*************"<<endl;
}