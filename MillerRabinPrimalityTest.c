#include<stdio.h>
#include<gmp.h>
#include<stdlib.h>
#include<time.h>


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
   
//the main function 
int main(int argc, char * argv[])
{
 if(argc!=3)
  {
   printf("Using %s: n a \n",argv[0]);
   exit(-1);
  }
  
int nx=atoi(argv[1]), t=atoi(argv[2]),res;
mpz_t n;
mpz_init(n);
mpz_set_ui(n,nx);

gmp_randstate_t generator ;//declaring a generator
gmp_randinit_default(generator); // Initializing the generator
gmp_randseed_ui(generator, time(NULL)); // giving the generator a seed
res=test_miller_rabin(n, t,generator );

//printing the result
switch(res)
 {
 case 0:
 gmp_printf("%Zd is probably prime\n",n);
 break;
 case 1:
 gmp_printf("%Zd is composite\n",n);
   break;
 }
 gmp_randclear (generator);//clearing the generator
 mpz_clear(n);
 return 0;
}
