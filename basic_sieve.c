#include<stdio.h>
#include<gmp.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include <sys/types.h>
#include <sys/resource.h>

//prime tester, returns 0 if prime and 1 otherwise 
int IsPrime(int x)
 { 
  int res=0;
  for(int i=2;i<=sqrt(x);i++)
   {
    if(x%i==0)
     {
      res=1;
      break;
     }
   }
   return res;
  }

//this function generates k primes strating from 2
void prime_generation( int arr[], int k)
 {
  arr[0]=2;
  int counter=1,i=3;
  while(counter<k)
   {
    if(IsPrime(i)==0)
     {
      arr[counter]=i;
      counter++;
     }
    i++;
  }
 }
 
//testing the divisibilty by all the items in the array, return 1 if n is divisble by anyone of the k primes 
int test_with_array(int arr[],mpz_t n,int k)
 {
  int res=0;
  for(int i=0;i<k;i++)
   {
    if(mpz_divisible_ui_p (n,arr[i] ))
     {
      res=1;
      break;
     }
   }
  return res;
 }
 
//the miller rabin test 
//writing n-1 as 2^s.r 
void form(mpz_t n, mpz_t r, mpz_t s)
 {
  //n1=n-1
  mpz_t n1,n2;
  mpz_inits(n1,n2,NULL);
  mpz_sub_ui(n1,n,1);
  mpz_set(n2,n1);//n2=n1
  //defining s
  int sx=0;
  while(mpz_divisible_ui_p (n1, 2))
   {
    mpz_cdiv_q_ui(n1, n1,2 );
    
    sx++;
   }
   mpz_set_ui(s,sx);
   //defining r
   mpz_ui_pow_ui (r, 2 , sx );
   mpz_cdiv_q (r, n2, r);
   mpz_clears(n1,n2,NULL);
  }
   
//testing the probable primality of n according to the base a  using miller rabin test 
int test_miller_rabin_base(mpz_t n, mpz_t a)
 {
  int j,res=0; 
  mpz_t y,n1,s1,r,s;
  mpz_inits(y,n1,s1,r,s,NULL);
  
  
  form(n, r, s);//writing n-1 as 2^s.r 
  mpz_sub_ui(n1,n,1);//n1=n-1
  mpz_sub_ui(s1,s,1);//s1=s-1
  mpz_powm(y,a,r,n);
  
  //the test 
  if((mpz_cmp_ui(y,1)!=0)&&(mpz_cmp(y,n1)!=0))
   {
    j=1;
    while((mpz_cmp_ui(s1,j)>=0)&&(mpz_cmp(y,n1)!=0))
     {
      mpz_powm_ui(y,y,2,n);
      if(mpz_cmp_ui(y,1)==0)
       {
        res=1;
       }
      j++;
     }
    if(mpz_cmp(y,n1)!=0)
     {
      res=1;
     }
    }
   //clearing the excess
  mpz_clears(y,n1,s1,r,s,NULL);
  return res;
 }

int test_miller_rabin(mpz_t n, int t, gmp_randstate_t generator)
 {
  int res;
  mpz_t a,n1;
  mpz_inits(a,n1,NULL);
  mpz_sub_ui(n1,n,1);
  for(int i=1;i<=t;i++)
   {
    mpz_urandomm (a,generator, n1);
    if(mpz_cmp_ui(a,2)<0)
     {
      mpz_add_ui(a,a,2);
     }
    res=test_miller_rabin_base(n,a);
   }
   return res;
  }
//the basic sieve function
void basic_sieve(mpz_t q, int k, gmp_randstate_t generator, int arr[], int b, int t)
 {
  mpz_t a; //declaring the variable n
  mpz_init(a); //initializing the variable n
  mpz_ui_pow_ui (a, 2, b-1);
  do
  {
   mpz_urandomb (q, generator ,b-1 ); //generate an integer n of at most k bits
   mpz_add(q,q,a);
  }while(mpz_even_p (q ));
  prime_generation(arr,k);//generate an array of k primes 

  while(test_with_array(arr,q,k)!=0)
   {
    mpz_add_ui(q,q,2);
   }
  
  while(test_miller_rabin(q,t,generator)!=0)
   {
    mpz_add_ui(q,q,2);
    while(test_with_array(arr,q,k)!=0)
     {
     mpz_add_ui(q,q,2);
     }
   }
   
   mpz_clear(a);
 }
 
unsigned long int cputime()
{
  struct rusage rus;

  getrusage (0, &rus);
  return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}
 
int main(int argc, char *argv[])
 {
  if (argc!=5)
   {
    printf("Using %s: b k t r\n",argv[0]);
    exit(-1);
   }
  int b=atoi(argv[1]),k=atoi(argv[2]),t=atoi(argv[3]),r=atoi(argv[4]),arr[k];
  unsigned long int start;
  start = cputime();
  mpz_t q,z_n[r];
  mpz_init(q);
  
  //the random generator 
  gmp_randstate_t generator ;//declaring a generator
  gmp_randinit_default(generator); // Initializing the generator
  gmp_randseed_ui(generator, time(NULL)); // giving the generator a seed   
  
  //the result
  //dec+in
for(int i=0;i<r;i++)
 {
  mpz_t z_n[i];
 }
for(int i=0;i<r;i++)
 {
   mpz_init(z_n[i]);
 }

for(int i=0;i<r;i++)
 { 
  //printf("test %d\n",i);
  basic_sieve(z_n[i],k, generator, arr, b, t);
  //gmp_printf("the probable prime generater by the basic sieve is %Zd\n",z_n[i]);
 }
printf("Computation time : %lu ms\n", cputime() - start);  
for(int i=0;i<r;i++)
 {
  mpz_clear(z_n[i]);
 }
  
  gmp_randclear (generator);//clearing the generator
  mpz_clear(q);
  return 0;
  }
   
