#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <fstream>

using namespace std;

#ifndef biomanH
#define biomanH

//---------------------------------------------------------------------------
#define M_Compartment_Max 100                                                  // Memory for arrays that are set statically
#define M_Count_Memory 20000                                                   // Memory for arrays that are set dynamically
#define M_Count_53 53                                                          // Number of compartments = 53
#define M_Count_Cylinder_8 8                                                   // Number of body parts (considering symmetry)
#define M_Count_Cylinder_14 14                                                 // Number of body parts (without symmetry)
#define M_Count_13 13                                                          // Number of man parameters
#define M_Total_Graph 50                                                       // Number of total graphs
#define M_json_i_clo 10                                                        // Position of clothing parameters in the list
#define M_json_i_water 18                                                      // Position of immersion parameters in the list
#define String_Convert_Size 128

#define KOEFF 0.86                                                             // [1 kcal/h] = KOEFF * [1 W]
#define FCL 1.97                                                               // if RF is expressed in [m^2·°C/W], fcl=1.00+1.97*RF
#define EPS 0.000001                                                           // <=EPS equivalently ==0 // >=EPS equivalently !=0
//---------------------------------------------------------------------------


class bioman
{

private:
    //***___MAN___***//
    int m;                                                                      // number of compartments
    int m_clothes;                                                              // number of compartments with clothes.  34 or 67 ... (75)
    int i_man[2][M_Count_Cylinder_14];
    int i_clothes[M_Count_Cylinder_14];
    int i_torso, i_arm;                                                         // number of torso, arm.

    string text_man[2][M_Compartment_Max];

    double mr[M_Compartment_Max];                                               // metabolic rate - уровень метаболизма
    double fla[M_Compartment_Max];                                              // arterial blood flow - артериальный кровоток
    double flv[M_Compartment_Max];                                              // venous blood flow - венозный кровоток
    double ka[M_Compartment_Max];
    double kv[M_Compartment_Max];
    double e[M_Compartment_Max];                                                //
    double te[M_Compartment_Max];                                               // temperature, °C - температура
    double ma[M_Compartment_Max];                                               // mass of compartment, kg - масса
    double a[M_Compartment_Max];                                                // surface area, m^2 - площадь поверхности
    double c[M_Compartment_Max];                                                // specific heat, kcal/(kg °C) - удельная теплоемкость
    double hc[M_Compartment_Max];
    double hr[M_Compartment_Max];
    double qhk[M_Compartment_Max];                                              // thermal conductivity between the layers - теплопроводность между слоями (ранее K)

    double mr_start[M_Compartment_Max], fla_start[M_Compartment_Max], flv_start[M_Compartment_Max],
        ka_start[M_Compartment_Max], kv_start[M_Compartment_Max], e_start[M_Compartment_Max],
        te_start[M_Compartment_Max], ma_start[M_Compartment_Max], a_start[M_Compartment_Max],
        c_start[M_Compartment_Max], hc_start[M_Compartment_Max], hr_start[M_Compartment_Max],
        k_start[M_Compartment_Max];

    double Ref[M_Compartment_Max], Rf[M_Compartment_Max];
    double thickness[M_Compartment_Max], ip[M_Compartment_Max];
    double qh_clo[M_Compartment_Max];                                           // heat transfer from clothes in environment
    double Aclo_pct[M_Compartment_Max];                                         // Clothing surface area as a percentage of skin surface area

    double* man_parameters[M_Count_13];                                         // man parameters
    double* man_parameters_start[M_Count_13];                                   // initial man parameters
    double k_mhp[M_Count_Cylinder_14];                                          // Coefficients shivering
    double k_ext_air[M_Count_Cylinder_14];                                      // Coefficients load (exercise) distribution in air
    double k_ext_all[9][M_Count_Cylinder_14];                                   // DB of coefficients load (exercise) distribution
    double FLS_coef[M_Count_Cylinder_14];                                       // Coefficients distribution skin blood flow (local skin)

    double sum_a_man;
    double sum_mr_man;                                                          // Sum of initial metabolic rate
    double sum_ma_man;                                                          // Sum of mass
    double sum_flv_man;                                                         // Sum of volume blood flow
    double sum_ma_fat;                                                          // Sum of mass fat
    double sum_ma_muscle;                                                       // Sum of mass muscles
    double sum_ma_skin;                                                         // Sum of mass skin
    double BF;                                                                  // Percentage of body fat (%BF)
    double Tsk;                                                                 // Mean skin temperature
    double Tbr_start;                                                           // Initial value of brain temperature
    double Tin_org_start;                                                       // Initial value of internal organs temperature
    double Tsk_start;                                                           // Initial value of mean skin temperature
    double Tb_start;                                                            // Initial value of blood temperature
    double MR_m_start;                                                          // Sum of initial metabolic rate in all skeletal muscles
    double FLV_start;                                                           // Sum of initial blood flow
    double FLV_m_start;                                                         // Sum of initial blood flow in all skeletal muscles
    double FLV_sk_start;                                                        // Sum of initial blood flow in skin
    double e_skin_start;                                                        // Sum of initial skin evaporation
    double MR_sk;                                                               // Sum of skin metabolic rate
    double flv_mean;

    double esw, h_r, sw, r;
    double ro, ce, tb, bv, fls;

    void Reset_Data_Man(void);                                                  // Reset of data and setting initial values
    void Calc_Man(void);                                                        // Calculating sum of man parameters
    void Muscle_calc(void);                                                     // Metabolic rate and blood flow in all skeletal muscles
    void Skin_calc(void);                                                       // Calculation of mean skin temperature
    void Coef_Skin_Blood_flow_calc(void);                                       // Calculation of coefficients distribution skin blood flow
    void Man_start(void);                                                       // Save start parameters of man
    void Blood_calc(void);                                                      // Calculating sum of blood flow
    void Local_BF_skin_calc(void);                                              // Local distribution of skin blood flow
    void Creation_K_exercise(void);                                             // Creation of database of coefficients load (exercise) distribution
    //**********//

    //***___Environment___***//
    bool env_local_flag;

    double total_ta, total_rh, total_v;
    double ta[M_Compartment_Max];
    double ta_air[M_Compartment_Max];
    double rh[M_Compartment_Max];
    double rh_air[M_Compartment_Max];
    double v[M_Compartment_Max];
    double v_air[M_Compartment_Max];
    double Penv[M_Compartment_Max];

    void SaveEnv(void);                                                         // Environment
    void Calc_Penv(void);
    //**********//

    //***___Immersion___***//
    void No_Immersion(void);
    void Input_Immersion(void);
    void Open_Immersion(void);

    double temp_water, t_on_water, t_off_water;
    double t_air_imm, rh_air_imm, v_air_imm;
    double hc_imm_fr[M_Compartment_Max];                                        // without exercise
    double hc_imm_f0_ex[M_Compartment_Max];                                     // with exercise
    double sum_a_man_air;
    int Qe_water;                                                               // Intensity of exercise in water
    double k_ext_water[M_Count_Cylinder_14];                                    // Coefficients load (exercise) distribution in water
    int flag_water;                                                             // Flag of Immersion parameters, water
    bool locus_immersion[M_Compartment_Max];
    bool Water_Level[M_Count_Cylinder_14];                                      // Immersion level
    double hc_water;                                                            // Coefficient of water convection (hc)
    double KPD_water;                                                           // For swimming KPD 4-7% (max 15%)
    //**********//

    //***___BIOMAN___***//
    int t_step;                                                                 // Counter of step
    int i_color;                                                                // Number of color graphs
    int Qe, Qe_air;                                                             // Intensity of exercise
    int flag_exercise_air;
    int i_type, i_sub_type;                                                     // type_activities, sub_type_activities
    double t_h;                                                                 // Time step
    double t_phase;                                                             // Time of  phase experiment
    double t_ex_start, t_ex_finish;                                              // Intervals of exercise
    double t_cold_shiver;                                                       // Time of cold shiver
    double T_sh_max;                                                            // Maximum time of shivering
    double Heat_coef_work;                                                      // Heat coefficient of exercise
    double KPD_air;                                                             // For running KPD about 20%
    double v_activity_air;                                                      // Activity velocity, m/s
    double WL;                                                                  // Water losses, g
    double Sw_eff;                                                              // Sweat efficiancy
    double Msh;                                                                 // Shivering [kcal/h], Cold thermogenesis in skeletal muscles
    double* k_ext;                                                              // Coefficients load (exercise) distribution

    double* temp[M_Compartment_Max];
    double* Evap_sw_rate_loc[M_Compartment_Max];
    double* Dripping_loc[M_Compartment_Max];
    double* Wettedness_loc[M_Compartment_Max];
    double* Blood_flow_loc[M_Compartment_Max];
    double* Shivering_loc[M_Compartment_Max];
    double* Real_evap_loc[M_Compartment_Max];
    double* Possible_evap_loc[M_Compartment_Max];
    double* Activity_MET_loc[M_Compartment_Max];

    double* Time_X;                                                             // Time, array
    double* Blood_T;                                                            // Blood temperature, array
    double* MSkinT, * MBodyT, * MMusclesT, * MFatT;                                // Mean temperature, arrays
    double* Heart_Rate;                                                         // Heart rate, array
    double* Cardiac_output;
    double* Stroke_volume;

    double* Total_metabolism;
    double* Metabolism_skin;
    double* Skin_heat_flow;
    double* Skin_conduction;
    double* M_Radiation;
    double* Air_convection;
    double* Water_convection;
    double* Respiratory_losses;
    double* M_Balance;

    double* Required_evaporation;
    double* Possible_evaporation;
    double* Real_evaporation;
    double* Dripping_sw_rate;
    double* Evaporation_sw_rate;
    double* Water_losses;
    double* M_Wettedness;
    double* Sweat_efficiancy;
    double* Skin_blood_flow;
    double* Muscles_blood_flow;
    double* Shivering_total;
    double* Metabolic_rate;

    double* Heat_Transmission_M6;                                               // M6 ???

    double* t_air_gr;
    double* t_water_gr;
    double* Qe_gr;
    int* RiskBodyTemp;
    int* RiskWaterLoses;

    bool flag_evaporation;                                                      // false (= 0) - not evaporation. true (= 1) - yes evaporation.
    double t;                                                                   // Time
    double convergence;                                                         // convergence preliminary calculation
    double ework;                                                               // Equivalent additional physical activity in kcal/h
    double MR_total;                                                            // Sum of metabolic rate
    double s3;
    double wet;                                                                 // Wettedness
    double Tot_R, deri, ESWmax;
    double mi, des, dev, flm;
    double mmust;                                                               // Mean muscle temperature
    double mft;                                                                 // Mean fat temperature
    double sq, sq2, sq3, sq4, sq4_w, sm, skm;
    double sqn, sqn3, sqn4, smn;
    double ESWW;                                                                // Real evaporation, kcal/h
    double SWdrip, SWdrip_loc[M_Compartment_Max];                               // Dripping sweat rate, g/h
    double Mwork_loc[M_Compartment_Max];                                        // Delta activity metabolic rate (without basal metabolism)
    double Msh_loc[M_Compartment_Max];                                          // Local shivering
    double esw_loc[M_Compartment_Max], ESWW_loc[M_Compartment_Max];
    double heeq[M_Compartment_Max], ESWm[M_Compartment_Max], we[M_Compartment_Max], Pa[M_Compartment_Max];

    void Initial_Value(void);                                                   // Setting initial man and environment parameters
    void Create_Data(int);                                                      // Dynamic allocation of memory for data
    void Delete_Data(void);                                                     // Delete of dynamically allocated memory for data
    void Bio_Change_Run(void);                                                  // Simulation experiment
    void Equivalent_he(void);                                                   // Calculation of coefficient he[i]
    void Clone_Parameters(void);                                                // Changing array of parameters when administered clothes
    void Man_Clothes(void);                                                     // Changing array of parameters when administered clothes
    void No_Clothes(void);                                                      // Reset of clothing data
    void Immersion_add(void);                                                   // Introduction of immersion
    void BuilderGraphBP(void);                                                  // Forming initial arrays to display graph

    void Reset_Data_1(void);                                                    // Reset of data and setting initial values
    void Reset_Data_2(void);                                                    // Reset of data and setting initial values
    void BuilderGraphAP(void);                                                  // Forming arrays to display graph
    void Series_Std_St_calc(void);                                              // proverka usloviya ustanovivshegosya rejima
    void Skin_flows_calc(void);                                                 // Heat skin flows
    void Threshold_All_Evapor(void);                                            // Check the condition of the appearance of regulatory evaporation and calculation of unevaporated moisture
    void Shivering_heat(void);                                                  // Cold thermogenesis (Shivering) in skeletal muscles
    //void Function8(void);                                                     // Cold skin flows
    void Stationar(void);                                                       // Calculation of local temperature, average temperatures, some heat flows
    void Calc1(void);
    void Physical_evapor_calc(void);                                            // Calculation of maximum evaporation from surface of body, based on experimental conditions
    void Required_evapor_calc(void);                                            // Calculation physiologically required (regulatory) evaporation
    void Real_evaporation_calc(void);                                           // Calculation of real evaporation (total/local) and derivative values
    void Real_evap_local(void);                                                 // Calculation of local real evaporation
    void Dripping_calc(void);                                                   // Calculation of water loss through sweat dripping
    void Restriction_Tcl(void);
    void Exercise_add(void);                                                    // Introduction of physical activity (load, exercise)
    void Energy_Cost_Run(void);                                                 // Type of physical activity - running

    //**********//

    //***___Results___***//
    double** Total_gr[M_Total_Graph];
    vector <string> Title_total_gr;
    vector <string> ABR_total_gr;
    vector <string> Measure_total_gr;
    int n_total_gr;
    //**********//


    //***___Form_Thermoregulation___***//
    double h11, h12, h31, h32, h41, h42, h43;
    int h13, h14, h33, h34, h44;
    double s13, s14, s31, s32, s33, s34, s41, s42, s43, s44;
    int am1, ah1, am2, ah2, am3, ah3, am4, ah4;
    double hdev, hdes, kw, kt;
    double fls_max, fls_min;
    bool flag_Head_skin_flow;                                                   // Form_Thermoregulation->CheckBox_head_FLS
    bool flag_flv_a;                                                            // Flag of Coef_fls = a_i/A
    void Initial_Contr_Resp(void);                                              // Setting initial values of thermoregulation parameters

    //**********//

    //***___Form_Details_Clothing___***//
    double Ref_a[M_Count_Cylinder_14];
    double Rf_a[M_Count_Cylinder_14];
    double ip_a[M_Count_Cylinder_14];
    double density_a[M_Count_Cylinder_14];
    double thickness_a[M_Count_Cylinder_14];
    double Aclo_pct_a[M_Count_Cylinder_14];

    double Ref_w[M_Count_Cylinder_14];
    double Rf_w[M_Count_Cylinder_14];
    double ip_w[M_Count_Cylinder_14];
    double density_w[M_Count_Cylinder_14];
    double thickness_w[M_Count_Cylinder_14];
    double Aclo_pct_w[M_Count_Cylinder_14];

    bool flag_cloth;                                                            // indicator of presence of clothes
    int Cloth_code[M_Count_Cylinder_14];
    void fOpen_FABRIC_current_Csv(void);                                        // Open FABRIC_current.csv
    void fReading_Cloth(string);
    //**********//



    //EMF_class emf;
    //**********//

    void fLine_Android_Results(vector <string>&, int, string);                 // Forming Line in Format JSON to Save Experiment Results for Android
    void fLine_Android_Results_WATER(vector <string>&, int, string);           // Forming Line in Format JSON to Save Experiment Results for Android WATER
    void fLine_Web_Results(vector <string>&, int, string);                     // Forming Line in Format JSON to Save Experiment Results for Web-app
    string fDouble_to_String(double);
    string fInt_to_String(int);
    double fString_to_Double(string);
    void fSaving_Results(string&, string);                                     // Saving Results_short.json

public:
    bool flag_Recalc;
    bool flag_error_b;                                                          // false (= 0) - not error. true (= 1) - ERROR!!!
    bool flag_immersion;                                                        // water environment - true (= 1), air environment - false (= 0)
    int flag_android;                                                           // from_android: true (= 1) - android, false (= 0) - web-app .
    string Text_Log_Error_b;                                                    // Text of errors in the execution of functions (class bioman)
    vector <string> Values_js;                                                  // Values from JSON to class bioman
    string Data_FABRIC_Path;                                                    // Path to FABRIC_current.csv
    string Global_Results;

    void Initial_New_Exp_0(void);                                               // Initial a new experiment
    void SteadyState(void);                                                     // The transition to steady state
    int Input_Exp_Param(void);                                                  // Input of experiment data from JSON file to model (class bioman)
    void Start_New_Exp_0(void);                                                 // Modeling the experiment
    void Save_Table_Results(string);                                            // Saving the Experiment Results Table
    void Android_JSON_Results(string);                                          // Saving the Experiment Results to JSON file for Android
    void Android_JSON_Results_WATER(string);                                    // Saving the Experiment Results to JSON file for Android WATER
    void Web_JSON_Results(string);                                              // Saving the Experiment Results to JSON file for Web-app
    void fSaving_Exp_Cond_Json(string);                                         // Saving Experimental_conditions.json


};
#endif

