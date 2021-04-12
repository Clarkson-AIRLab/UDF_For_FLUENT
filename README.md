# UDF_For_FLUENT
There are UDF files for FLUENT, including particle drag force and electrical field.
Notice that:
1. The "drag_test_n_plus_1.c","udf.h" and "udfconfig.h" files are the old UDF files that can create electric field, electric force and a correction of the drag force in Stokes regime. The pdf file is a tutorial for using these three files to build the actual code for FLUENT.
2. The pdf file includes how to use udf for BCs, drag force and scalar-update. You also need a tool from visual studio in order to build the code in FLUENT (details in this pdf file). However, this tool's name is changing every year as visual studio keep updating. In pdf file it calls Visual Studio x64 Cross Tools Command Window, what I am using is called native tools.
3. It seems like creating the udf files in FLUENT can only work for academic version, not wroking for education version.
4. "drag_E_Cd_Cc_sf.c" is the udf file we recently modified for correcting drag force beyond Stokes regime and can consider the shape factor. If your simulations involve the particle Re change from <0.1 to <1000, feel free to try this udf file. 
5. For using the shape factor in "drag_E_Cd_Cc_sf.c" udf file, the shape factor named as "sf" in code, the drag coefficient is considered the equations from Fluent and added Cc correction into it. If you want to consider the shape factor, you need to change the value of shape factor in the file and build a new udf library with the new shape factor in FLUENT. 
