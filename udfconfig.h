/*
 *	Copyright 1997 Fluent Inc.
 *	All Rights Reserved
 *
 *	This is unpublished proprietary source code of Fluent Inc.
 *	It is protected by U.S. copyright law as an unpublished work
 *	and is furnished pursuant to a written license agreement.  It
 *	is considered by Fluent Inc. to be confidential and may not be
 *	used, copied, or disclosed to others except in accordance with
 *	the terms and conditions of the license agreement.
 */

#define UDF_COMPILER 1

#define EOF (-1)
#define NULL (0)

#define const
#define register


typedef unsigned int size_t;



/* config.h */

#define RP_2D 1
#define RP_3D 0
#define RP_HOST 0
#define RP_NODE 0
#define RP_DOUBLE 0
#define RP_STORE_FACE_AVE 1
#define RP_STORE_CELL_AVE 1

#define STRUCT_OFF(t,offset,type)((type*)((char *)(t)+offset))
#define STRUCT_REF(t,offset,type)(*STRUCT_OFF(t,offset,type))

#define DOMAIN_C(t)STRUCT_REF(t,0,Thread *)
#define DOMAIN_NOSOLVE_C(t)STRUCT_REF(t,4,Thread *)
#define DOMAIN_NOSOLVE_EXT_C(t)STRUCT_REF(t,8,Thread *)
#define DOMAIN_F(t)STRUCT_REF(t,12,Thread *)
#define DOMAIN_NOSOLVE_F(t)STRUCT_REF(t,16,Thread *)
#define DOMAIN_ID(t)STRUCT_REF(t,700,int)
#define PHASE_DOMAIN_INDEX(t)STRUCT_REF(t,704,int)
#define DOMAIN_N_DOMAINS(t)STRUCT_REF(t,712,int)
#define DOMAIN_SUPER_DOMAIN(t)STRUCT_REF(t,804,Domain *)
#define DOMAIN_SUB_DOMAIN(t,i)(STRUCT_OFF(t,716,Domain *)[i])

#define THREAD_STORAGE(t,nv)(STRUCT_OFF(t,4,void *)[nv])
#define THREAD_T0(t)STRUCT_REF(t,24096,Thread *)
#define THREAD_T1(t)STRUCT_REF(t,24100,Thread *)
#define THREAD_NEXT(t)STRUCT_REF(t,24140,Thread *)
#define THREAD_ID(t)STRUCT_REF(t,24256,int)
#define THREAD_NAME(t)STRUCT_REF(t,23828,char *)
#define THREAD_N_ELEMENTS(t)STRUCT_REF(t,24088,int)
#define THREAD_FLAGS(t)STRUCT_REF(t,24260,int)
#define THREAD_SUB_THREAD(t,i)(STRUCT_OFF(t,24168,Thread *)[i])
#define THREAD_SUB_THREADS(t)STRUCT_OFF(t,24168,Thread *)
#define THREAD_SUPER_THREAD(t)STRUCT_REF(t,24164,Thread *)
#define THREAD_DOMAIN(t)STRUCT_REF(t,24268,Domain *)
#define THREAD_HEAD(t)STRUCT_REF(t,0,Thread_Head *)
#define THREAD_MATERIAL(t)(STRUCT_REF(THREAD_HEAD(t),41792,Material *))

#define NODE_COORD(v)STRUCT_OFF(v,0,real)

#define P_INIT_DIAM(p)STRUCT_REF(p,196,real)
#define P_INIT_MASS(p)STRUCT_REF(p,204,real)
#define P_INIT_RHO(p)STRUCT_REF(p,200,real)
#define P_INIT_TEMP(p)STRUCT_REF(p,208,real)
#define P_INIT_LF(p)STRUCT_REF(p,216,real)
#define P_VFF(p)STRUCT_REF(p,1516,real)
#define P_DT(p)STRUCT_REF(p,36,real)
#define P_DIAM(p)STRUCT_REF(p,292,real)
#define P_T(p)STRUCT_REF(p,304,real)
#define P_VEL(p)STRUCT_OFF(p,280,real)
#define P_RHO(p)STRUCT_REF(p,296,real)
#define P_LF(p)STRUCT_REF(p,312,real)
#define P_MASS(p)STRUCT_REF(p,300,real)
#define P_TIME(p)STRUCT_REF(p,308,real)
#define P_DIAM0(p)STRUCT_REF(p,244,real)
#define P_VEL0(p)STRUCT_OFF(p,232,real)
#define P_T0(p)STRUCT_REF(p,256,real)
#define P_RHO0(p)STRUCT_REF(p,248,real)
#define P_LF0(p)STRUCT_REF(p,264,real)
#define P_MASS0(p)STRUCT_REF(p,252,real)
#define P_TIME0(p)STRUCT_REF(p,260,real)
#define P_CURRENT_LAW_INDEX(p)STRUCT_REF(p,128,int)
#define P_INJECTION(p)STRUCT_REF(p,164,Injection *)
#define P_EVAP_SPECIES_INDEX(p)STRUCT_REF(P_INJECTION(p),528,int)
#define P_DEVOL_SPECIES_INDEX(p)STRUCT_REF(P_INJECTION(p),532,int)
#define P_OXID_SPECIES_INDEX(p)STRUCT_REF(P_INJECTION(p),536,int)
#define P_PROD_SPECIES_INDEX(p)STRUCT_REF(P_INJECTION(p),540,int)
#define P_LAWS(p,in)STRUCT_OFF(P_INJECTION(p),92,int)[in]

#define MATERIAL_PROPERTY(m)STRUCT_OFF(m,64,Property)
#define PROPERTY_LENGTH 76
#define MATERIAL_PROPERTY_ELEMENT(m,i)((Property *)((char *)MATERIAL_PROPERTY(m)+PROPERTY_LENGTH*i))
#define MATERIAL_PROP(m,i)(STRUCT_REF(MATERIAL_PROPERTY_ELEMENT(m,i),4,float))
