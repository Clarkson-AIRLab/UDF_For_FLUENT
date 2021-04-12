/* %W% Copyright 19%D% Fluent Inc. */

/*
 *	Copyright 1997-1998 Fluent Inc.
 *	All Rights Reserved
 *
 *	This is unpublished proprietary source code of Fluent Inc.
 *	It is protected by U.S. copyright law as an unpublished work
 *	and is furnished pursuant to a written license agreement.  It
 *	is considered by Fluent Inc. to be confidential and may not be
 *	used, copied, or disclosed to others except in accordance with
 *	the terms and conditions of the license agreement.
 */

/* Header file for User Defined Functions */

#define _UDF 1

#ifdef UDFCONFIG_H
# include UDFCONFIG_H
#endif

#include "global.h"
#include "mem.h"
#include "para.h"
#include "threads.h"

#include "case.h"
#include "flow.h"
#include "id.h"
#include "metric.h"
#if PARALLEL
# include "pio.h"
#endif /* PARALLEL */
#include "prop.h"
#include "real_gas.h"
#include "lmain.h"

#include "sg_nox.h"
#include "sg_mphase.h"
#include "sg_ke.h"
#include "sg_disco.h"
#include "slide.h"
#include "dpm.h"
#include "rwdata.h"
#include "dynamesh_tools.h"

typedef struct UDF_Data
{
  char *name;
  void (*fcn)();                /* pointer to a function returning void */
  int type;
}
UDF_Data;


enum
{
  UDF_TYPE_DIFFUSIVITY,		/* per thread or per equation functions */
  UDF_TYPE_PROFILE,
  UDF_TYPE_PROPERTY,
  UDF_TYPE_EXCHANGE_PROPERTY,
  UDF_TYPE_VECTOR_EXCHANGE_PROPERTY,
  UDF_TYPE_SOURCE,
  UDF_TYPE_DPM_BC,
  UDF_TYPE_INIT,		/* single definition functions */
  UDF_TYPE_ADJUST,
  UDF_TYPE_UDS_FLUX,
  UDF_TYPE_UDS_UNSTEADY,
  UDF_TYPE_HEAT_FLUX,
  UDF_TYPE_NET_REACTION_RATE,
  UDF_TYPE_VR_RATE,
  UDF_TYPE_SR_RATE,
  UDF_TYPE_PR_RATE,
  UDF_TYPE_TURB_PREMIX_SOURCE,
  UDF_TYPE_DPM_BODY_FORCE,
  UDF_TYPE_DPM_DRAG,
  UDF_TYPE_DPM_SOURCE,
  UDF_TYPE_DPM_OUTPUT,
  UDF_TYPE_DPM_EROSION,
  UDF_TYPE_DPM_LAW,
  UDF_TYPE_DPM_SWITCH,
  UDF_TYPE_SCAT_PHASE_FUNC,
  UDF_TYPE_DPM_SCALAR_UPDATE,
  UDF_TYPE_DPM_SPRAY_COLLIDE,
  UDF_TYPE_DPM_INJECTION_INIT,
  UDF_TYPE_DPM_PROPERTY,
  UDF_TYPE_TURBULENT_VISCOSITY,
  UDF_TYPE_PRANDTL_K,
  UDF_TYPE_PRANDTL_D,
  UDF_TYPE_PRANDTL_O,
  UDF_TYPE_PRANDTL_T,
  UDF_TYPE_PRANDTL_T_WALL,
  UDF_TYPE_CAVITATION_RATE,
  UDF_TYPE_RW_FILE,
  UDF_TYPE_ON_DEMAND,
  UDF_TYPE_EXECUTE_AT_END,
  UDF_TYPE_NOX_RATE,
  UDF_TYPE_CHEM_STEP,
  UDF_TYPE_PDF_TRANSPORT_DENSITY,
  UDF_TYPE_GRID_MOTION,
  UDF_TYPE_CG_MOTION,
  UDF_TYPE_GEOM,
  UDF_TYPE_HXC_EFFECTIVENESS,
  UDF_TYPE_DELTAT,
  UDF_TYPE_DOM_SOURCE,
  UDF_TYPE_DOM_BC_ADJUST,
  UDF_TYPE_DOM_DIFFUSE_REFLECTIVITY,
  UDF_TYPE_DOM_SPECULAR_REFLECTIVITY,
  UDF_TYPE_NETWORK_PROP,
  UDF_TYPE_EVAL
};

#define DEFINE_DIFFUSIVITY(name, c, t, i) real name(cell_t c, Thread *t, int i)
#define DEFINE_PROFILE(name, t, i) void name(Thread *t, int i)
#define DEFINE_PROPERTY(name, c, t) real name(cell_t c, Thread *t)
#define DEFINE_DPM_PROPERTY(name, c, t, p) \
  real name(cell_t c, Thread *t, Tracked_Particle *p)
#define DEFINE_EXCHANGE_PROPERTY(name, c, mixture_thread, second_column_phase_index, first_column_phase_index) \
real name(cell_t c, Thread *mixture_thread, int second_column_phase_index, int first_column_phase_index)
#define DEFINE_VECTOR_EXCHANGE_PROPERTY(name, c, mixture_thread, second_column_phase_index, first_column_phase_index, vector_result) \
void name(cell_t c, Thread *mixture_thread, int second_column_phase_index, int first_column_phase_index, real *vector_result)
#define DEFINE_SOURCE(name, c, t, dS, i) \
  real name(cell_t c, Thread *t, real dS[], int i)
#define DEFINE_INIT(name, domain) void name(Domain *domain)
#define DEFINE_ADJUST(name, domain) void name(Domain *domain)
#define DEFINE_UDS_FLUX(name, f, t, i) real name(face_t f, Thread *t, int i)
#define DEFINE_UDS_UNSTEADY(name, c, t, i, apu, su) \
  void name(cell_t c, Thread *t, int i, real *apu, real *su)
#define DEFINE_HEAT_FLUX(name, f, t, c0, t0, cid, cir) \
  void name(face_t f, Thread *t, cell_t c0, Thread *t0, real cid[], real cir[])
#define DEFINE_VR_RATE(name, c, t, r, mw, yi, rr, rr_t) \
  void name(cell_t c, Thread *t, \
	    Reaction *r, real *mw, real *yi, real *rr, real *rr_t)
#define DEFINE_NET_REACTION_RATE(name, p, temp, yi, rr, jac) \
  void name(double *p, double *temp, double *yi, double *rr, double *jac)
#define DEFINE_SR_RATE(name, f, t, r, mw, yi, rr) \
  void name(face_t f, Thread *t, \
	    Reaction *r, real *mw, real *yi, real *rr)
#define DEFINE_PR_RATE(name, c, t, r, mw, ci, p, sf, dif_index, cat_index, rr) \
  void name(cell_t c, Thread *t, \
	    Reaction *r, real *mw, real *ci, Tracked_Particle *p, \
        real *sf , int dif_index, int cat_index, real *rr)
#define DEFINE_TURB_PREMIX_SOURCE(name, c, t, turbulent_flame_speed, source) \
  void name(cell_t c, Thread *t, real *turbulent_flame_speed, real *source)
#define DEFINE_DPM_BODY_FORCE(name, p, i) real name(Tracked_Particle *p, int i)
#define DEFINE_DPM_DRAG(name, Re, p) real name(real Re, Tracked_Particle *p)
#define DEFINE_DPM_SOURCE(name, c, t, S, strength, p) \
  void name(cell_t c, Thread *t, dpms_t *S, real strength, Tracked_Particle *p)
#define DEFINE_DPM_OUTPUT(name, header, fp, p, t, plane) \
  void name(int header, FILE *fp, Tracked_Particle *p, Thread *t, Plane *plane)
#define DEFINE_DPM_EROSION(name, p, t, f, normal, alpha, Vmag, mdot) \
  void name(Tracked_Particle *p, Thread *t, face_t f, real normal[], \
	    real alpha, real Vmag, real mdot)
#define DEFINE_DPM_SCALAR_UPDATE(name, c, t, initialize, p) \
  void name(cell_t c, Thread *t, int initialize, Tracked_Particle *p)
#define DEFINE_DPM_SPRAY_COLLIDE(name, tp, p) \
  void name(Tracked_Particle *tp, Particle *p)
#define DEFINE_DPM_LAW(name, p, ci) void name(Tracked_Particle *p, int ci)
#define DEFINE_DPM_SWITCH(name, p, ci) void name(Tracked_Particle *p, int ci)
#define DEFINE_DPM_INJECTION_INIT(name, I) void name(Injection *I)
#define DEFINE_DPM_BC(name, p, t, f, normal, dim) \
  int name(Tracked_Particle *p, Thread *t, face_t f, real normal[], int dim)
#define DEFINE_SCAT_PHASE_FUNC(name, c, f) \
     real name(real c, real *f)
#define DEFINE_CAVITATION_RATE(name, c, t, p, rhoV, rhoL, vofV, p_v, n_b, m_dot) \
  void name(cell_t c, Thread *t, real *p, real *rhoV, real *rhoL, real *vofV, \
	    real *p_v, real *n_b, real *m_dot)
#define DEFINE_RW_FILE(name, fp) void name(FILE *fp)
#define DEFINE_ON_DEMAND(name) void name(void)
#define DEFINE_EXECUTE_AT_END(name) void name(void)
#define DEFINE_TURBULENT_VISCOSITY(name, c, t) real name(cell_t c, Thread *t)
#define DEFINE_PRANDTL_K(name, c, t) real name(cell_t c, Thread *t)
#define DEFINE_PRANDTL_D(name, c, t) real name(cell_t c, Thread *t)
#define DEFINE_PRANDTL_O(name, c, t) real name(cell_t c, Thread *t)
#define DEFINE_PRANDTL_T(name, c, t) real name(cell_t c, Thread *t)
#define DEFINE_PRANDTL_T_WALL(name, c, t) real name(cell_t c, Thread *t)
#define DEFINE_NOX_RATE(name, c, t, NOx) \
  void name(cell_t c, Thread *t, NOx_Parameter *NOx)
#define DEFINE_CHEM_STEP(name, ifail, n, dt, p, temp, yk) \
  void name(int *ifail, int n, real dt, real *p, real *temp, real *yk)
#define DEFINE_PDF_TRANSPORT_DENSITY(name, c, t, p) \
  real name(cell_t c, Thread *t, Particle *p)
#define DEFINE_GRID_MOTION(name, d, dt, time, dtime) \
  void name(Domain *d, Dynamic_Thread *dt, real time, real dtime)
#define DEFINE_CG_MOTION(name, dt, vel, omega, time, dtime) \
  void name(Dynamic_Thread *dt, real vel[], real omega[], real time, real dtime)
#define DEFINE_GEOM(name, d, dt, position) \
  void name(Domain *d, Dynamic_Thread *dt, real *position)
#define DEFINE_DELTAT(name, domain) real name(Domain *domain)

#define DEFINE_DOM_SOURCE(name,c,t,ni,nb,emission,in_scattering,abs_coeff,scat_coeff)\
  void name(cell_t c, Thread* t, int ni, int nb, real *emission, real *in_scattering,\
             real *abs_coeff, real *scat_coeff)
#define DEFINE_DOM_DIFFUSE_REFLECTIVITY(name,t,nb,n_a,n_b,diff_ref_a, \
diff_tran_a, diff_ref_b, diff_tran_b) \
void name(Thread *t, int nb,  real n_a, real n_b, real *diff_ref_a, \
real *diff_tran_a, real *diff_ref_b, real *diff_tran_b)

#define DEFINE_DOM_SPECULAR_REFLECTIVITY(name,f,t,nb,n_a, n_b, \
ray_direction,  e_n, total_internal_reflection, \
specular_reflectivity, specular_transmissivity) \
 void name(face_t f, Thread *t, int nb,  real n_a, real n_b , \
real ray_direction[], real e_n[],  int *total_internal_reflection, \
real *specular_reflectivity,  real *specular_transmissivity)

/* functions not yet available in the standard interface */
#define DEFINE_DOM_BC_ADJUST(name, domain) void name(Domain *domain)
#define DEFINE_NETWORK_PROP(name, network_id, network_cell_id) real name(int network_id, int network_cell_id)

#define DEFINE_EVAL(name, domain) \
 boolean name(Domain *domain)

/* backward compatibility */
#define C_PART(c,t) C_PART_ACTIVE(c,t)



/* end of udf.h */
