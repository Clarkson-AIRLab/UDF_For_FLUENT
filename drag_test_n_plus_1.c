#include "udf.h"
#include "dpm.h"
#include "surf.h"
#include "global.h"
#include "mem.h"
#define n 1.0         		/* number of charges on a particle */
#define e 1.6E-19       	/* Charge on a single electron, Coulombs */
#define TSTART 0.0   	/* field applied at t = tstart   */ 

cell_t in_c=-1;
Thread *in_t;
/*FILE *fp;*/
DEFINE_DPM_SCALAR_UPDATE(update_info, c, t, initialize, p)
{
  if (initialize)
    /* set in_c to a negative number so we don't use it if it is not set */
    in_c = -1;
  else
    {
      in_c = c;
      in_t = t;
   /* printf("cell=%d, thread=%d\n",c,t); */
    }
}

DEFINE_DPM_BODY_FORCE(particle_body_force_electrical, p, i)
{
 Domain *domain;
 cell_t in_c; 
 
 Thread *in_t;
 
 real bforce;
 domain = Get_Domain(1);
 if(P_TIME(p)>=TSTART)
 {
/*  fp=fopen("stokes_drag_bforce_pos.txt","a");*/ 
   thread_loop_c(in_t,domain)
       {
         begin_c_loop(in_c,in_t)
	     {		
             if(in_c==P_CELL(p) & in_t==P_CELL_THREAD(p))
               {
                 if(i == 0) 
                   {bforce=(n*e*C_UDSI_G(in_c,in_t,0)[0]);
               /*  fprintf(fp,"\n %g    %g",bforce,P_POS(p)[0]); */}
                  else if(i ==1) 
		       {bforce= n*e*C_UDSI_G(in_c,in_t,0)[1];
                 /* fprintf(fp,"         %g     %g",bforce,P_POS(p)[1]); */}  	 
                   }                     
               }
         end_c_loop(in_c,in_t)
    /*   printf(" \n bforce=%g\n",bforce);*/ 
       }
   /*    fclose(fp);*/
        
      /* printf("bforce=%g\n",bforce); */
	/* printf("P_MASS=%g\n",P_MASS(p));*/
  }	
  /* an acceleration should be returned */
        return (bforce/P_MASS(p));        
}


DEFINE_DPM_DRAG(drag, Re, p)
{
  real pres;
  real temp;
 
  real Cc = 1;
  cell_t use_c;
  Thread *use_t;
  
   if (in_c >= 0)
     {
       use_c = in_c;
       use_t = in_t; 
	 pres = 76+(76.0/101325.0)*C_P(use_c,use_t);
	 Cc = 1+2.0/(pres*(P_DIAM(p)*1E6))*(6.32+2.01*exp(-0.1095*pres*(P_DIAM(p)*1E6)));
     }
   printf("cc=%g\n",Cc);  
      return (18./Cc);	
}