#include "pch.h"
#include "bioman.h"


void bioman::Initial_New_Exp_0(void)
{
    Create_Data(M_Count_Memory);      // переделать, чтобы от времени зависело?
    Initial_Value();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//----------------------MAN--------------------------------------------------
//---------------------------------------------------------------------------

void bioman::Reset_Data_Man(void)
{
    for (int i = 0; i < M_Compartment_Max; i++)
    {
        for (int J = 0; J < M_Count_13; J++)
            *(man_parameters[J] + i) = 0;

        Ref[i] = 0;
        Rf[i] = 0;
        thickness[i] = 0;
        ip[i] = 0;
        Aclo_pct[i] = 0;
    }
    for (int J = 0; J < M_Count_Cylinder_14; J++)
    {
        i_man[0][J] = 0;
        i_man[1][J] = 0;
    }

    m_clothes = 0;
    i_torso = 0;
    i_arm = 0;

    ro = 1.056;               // kg / m^3
    ce = 0.93;                // J / (kg * °C)
    tb = 36.51;               // °C
    bv = 1.22;                // m^3              - circulating blood volume

    //Cb = 1.12;              // kcal / °C        - the thermal capacitance of the central blood
}
//---------------------------------------------------------------------------

void bioman::Initial_Value(void)
{
    double* man_inf[M_Count_13] = { mr, fla, flv, ka, kv, e, te, ma, a, c, hc, hr, qhk };

    double initial_value_man_53[M_Count_13][M_Count_53] = {
        /*mr*/      {0, 12.42, 0.12, 0, 40.0, 5, 2.34, 0.88, 0, 0.39, 0.1, 0.075, 0, 0.39, 0.1, 0.075, 0, 0.39, 0.05, 0.075, 0, 0.39, 0.05, 0.075, 0, 0.29, 0.04, 0, 0.29, 0.04, 0, 1.16, 0.27, 0.175, 0, 1.16, 0.27, 0.175, 0, 1.16, 0.13, 0.175, 0, 1.16, 0.13, 0.175, 0, 0.57, 0.06, 0, 0.57, 0.06, 0},
        /*fla*/     {0, 48, 1.6, 0, 230, 13.5, 2.3, 1, 0, 0.75, 0.107, 0.075, 0, 0.75, 0.107, 0.075, 0, 0.75, 0.053, 0.075, 0, 0.75, 0.053, 0.075, 0, 0.25, 2, 0, 0.25, 2, 0, 2, 0.267, 0.25, 0, 2, 0.267, 0.25, 0, 2, 0.133, 0.25, 0, 2, 0.133, 0.25, 0, 0.5, 1.5, 0, 0.5, 1.5, 0},
        /*flv*/     {0, 48, 1.6, 0, 230, 13.5, 2.3, 1, 0, 0.75, 0.107, 0.075, 0, 0.75, 0.107, 0.075, 0, 0.75, 0.053, 0.075, 0, 0.75, 0.053, 0.075, 0, 0.25, 2, 0, 0.25, 2, 0, 2, 0.267, 0.25, 0, 2, 0.267, 0.25, 0, 2, 0.133, 0.25, 0, 2, 0.133, 0.25, 0, 0.5, 1.5, 0, 0.5, 1.5, 0},
        /*ka*/      {0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0},
        /*kv*/      {0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0},
        /*e*/       {0, 4.5, 1.53, 0, 4.5, 0, 0, 3.37, 0, 0, 0, 0.17, 0, 0, 0, 0.17, 0, 0, 0, 0.55, 0, 0, 0, 0.55, 0, 0, 0.21, 0, 0, 0.21, 0, 0, 0, 0.55, 0, 0, 0, 0.55, 0, 0, 0, 0.6, 0, 0, 0, 0.6, 0, 0, 0.21, 0, 0, 0.21, 0},
        /*te*/      {0, 36.6, 34.6, 0, 36.7, 36.2, 35, 34, 0, 35.6, 35.1, 34.7, 0, 35.6, 35.1, 34.7, 0, 35.6, 35.1, 34.7, 0, 35.6, 35.1, 34.7, 0, 35.8, 35.6, 0, 35.8, 35.6, 0, 35.8, 35.7, 34.5, 0, 35.8, 35.7, 34.5, 0, 35.4, 35.1, 34, 0, 35.4, 35.1, 34, 0, 35.8, 35.5, 0, 35.8, 35.5, 0},
        /*ma*/      {0, 4.56, 0.27, 0, 24, 8.53, 6.8, 1.34, 0, 1.165, 0.325, 0.16, 0, 1.165, 0.325, 0.16, 0, 0.9, 0.16, 0.08, 0, 0.9, 0.16, 0.08, 0, 0.33, 0.1, 0, 0.33, 0.1, 0, 3.46, 0.8, 0.4, 0, 3.46, 0.8, 0.4, 0, 2.8, 0.4, 0.2, 0, 2.8, 0.4, 0.2, 0, 0.51, 0.46, 0, 0.51, 0.46, 0},
        /*a*/       {0, 0, 0.15, 0, 0, 0, 0, 0.68, 0, 0, 0, 0.075, 0, 0, 0, 0.075, 0, 0, 0, 0.055, 0, 0, 0, 0.055, 0, 0, 0.04, 0, 0, 0.04, 0, 0, 0, 0.135, 0, 0, 0, 0.135, 0, 0, 0, 0.115, 0, 0, 0, 0.115, 0, 0, 0.06, 0, 0, 0.06, 0},
        /*c*/       {0, 0.88052632, 0.81, 0, 0.8069, 0.9, 0.6, 0.81, 0, 0.9, 0.6, 0.81, 0, 0.9, 0.6, 0.81, 0, 0.9, 0.6, 0.81, 0, 0.9, 0.6, 0.81, 0, 0.9, 0.81, 0, 0.9, 0.81, 0, 0.9, 0.6, 0.81, 0, 0.9, 0.6, 0.81, 0, 0.9, 0.6, 0.81, 0, 0.9, 0.6, 0.81, 0, 0.9, 0.81, 0, 0.9, 0.81, 0},
        /*hc*/      {0, 0, 5.5, 0, 0, 0, 0, 4.5, 0, 0, 0, 2.25, 0, 0, 0, 2.25, 0, 0, 0, 2.25, 0, 0, 0, 2.25, 0, 0, 3, 0, 0, 3, 0, 0, 0, 2.25, 0, 0, 0, 2.25, 0, 0, 0, 2.25, 0, 0, 0, 2.25, 0, 0, 4, 0, 0, 4, 0},
        /*hr*/      {0, 0, 5.5, 0, 0, 0, 0, 4.5, 0, 0, 0, 2.25, 0, 0, 0, 2.25, 0, 0, 0, 2.25, 0, 0, 0, 2.25, 0, 0, 3, 0, 0, 3, 0, 0, 0, 2.25, 0, 0, 0, 2.25, 0, 0, 0, 2.25, 0, 0, 0, 2.25, 0, 0, 4, 0, 0, 4, 0},
        /*qhk*/     {0, 0, 2.63, 0, 0, 4.85, 20, 8, 0, 0, 14, 2.2, 0, 0, 14, 2.2, 0, 0, 14, 2.2, 0, 0, 14, 2.2, 0, 0, 2.88, 0, 0, 2.88, 0, 0, 25, 2.24, 0, 0, 25, 2.24, 0, 0, 25, 2.24, 0, 0, 25, 2.24, 0, 0, 2.16, 0, 0, 2.16, 0} };

    //*****
    // c*ma            {0, 4.0152, 0.2187, 0, 19.3656, 7.677, 4.08, 1.0854, 0, 1.0485, 0.195, 0.1296, 0, 1.0485, 0.195, 0.1296, 0, 0.81, 0.096, 0.0648, 0, 0.81, 0.096, 0.0648, 0, 0.297, 0.081, 0, 0.297, 0.081, 0, 3.114, 0.48, 0.324, 0, 3.114, 0.48, 0.324, 0, 2.52, 0.24, 0.162, 0, 2.52, 0.24, 0.162, 0, 0.459, 0.3726, 0, 0.459, 0.3726, 0},
    //!!!ma (BF_15%)   {0, 4.56, 0.27, 0, 24, 8.53, 6.8, 1.34, 0, 1.165, 0.325, 0.16, 0, 1.165, 0.325, 0.16, 0, 0.9, 0.16, 0.08, 0, 0.9, 0.16, 0.08, 0, 0.33, 0.1, 0, 0.33, 0.1, 0, 3.46, 0.8, 0.4, 0, 3.46, 0.8, 0.4, 0, 2.8, 0.4, 0.2, 0, 2.8, 0.4, 0.2, 0, 0.51, 0.46, 0, 0.51, 0.46, 0},
    //ma (BF_10%)      {0, 4.56, 0.27, 0, 24, 8.53, 4.444, 1.34, 0, 1.165, 0.212, 0.16, 0, 1.165, 0.212, 0.16, 0, 0.9, 0.105, 0.08, 0, 0.9, 0.105, 0.08, 0, 0.33, 0.1, 0, 0.33, 0.1, 0, 3.46, 0.523, 0.4, 0, 3.46, 0.523, 0.4, 0, 2.8, 0.261, 0.2, 0, 2.8, 0.261, 0.2, 0, 0.51, 0.46, 0, 0.51, 0.46, 0},
    //ma (BF_20%)      {0, 4.56, 0.27, 0, 24, 8.53, 10.003, 1.34, 0, 1.165, 0.478, 0.16, 0, 1.165, 0.478, 0.16, 0, 0.9, 0.235, 0.08, 0, 0.9, 0.235, 0.08, 0, 0.33, 0.1, 0, 0.33, 0.1, 0, 3.46, 1.177, 0.4, 0, 3.46, 1.177, 0.4, 0, 2.8, 0.588, 0.2, 0, 2.8, 0.588, 0.2, 0, 0.51, 0.46, 0, 0.51, 0.46, 0},


    string text_man_53[2][M_Count_53] = { {"", "head", "head", "", "torso", "torso", "torso", "torso", "", "arm", "arm", "arm", "", "arm", "arm", "arm", "", "forearm", "forearm", "forearm", "", "forearm", "forearm", "forearm", "", "hand", "hand", "", "hand", "hand", "", "thigh", "thigh", "thigh", "", "thigh", "thigh", "thigh", "", "calf", "calf", "calf", "", "calf", "calf", "calf", "", "foot", "foot", "", "foot", "foot", ""},
                                           {"", "brain", "skin", "", "internal organs", "muscles", "fat", "skin", "", "muscles", "fat", "skin", "", "muscles", "fat", "skin", "", "muscles", "fat", "skin", "", "muscles", "fat", "skin", "", "muscles", "skin", "", "muscles", "skin", "", "muscles", "fat", "skin", "", "muscles", "fat", "skin", "", "muscles", "fat", "skin", "", "muscles", "fat", "skin", "", "muscles", "skin", "", "muscles", "skin", ""} };

    double initial_value_env_53[3][M_Count_53] = {
          {0, 0, 30, 0, 0, 0, 0, 30, 0, 0, 0, 30, 0, 0, 0, 30, 0, 0, 0, 30, 0, 0, 0, 30, 0, 0, 30, 0, 0, 30, 0, 0, 0, 30, 0, 0, 0, 30, 0, 0, 0, 30, 0, 0, 0, 30, 0, 0, 30, 0, 0, 30, 0},
          {0, 0, 0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0.5, 0},
          {0, 0, 0.1, 0, 0, 0, 0, 0.1, 0, 0, 0, 0.1, 0, 0, 0, 0.1, 0, 0, 0, 0.1, 0, 0, 0, 0.1, 0, 0, 0.1, 0, 0, 0.1, 0, 0, 0, 0.1, 0, 0, 0, 0.1, 0, 0, 0, 0.1, 0, 0, 0, 0.1, 0, 0, 0.1, 0, 0, 0.1, 0} };

    int i, J, j_2;

    for (i = 0; i < M_Count_13; i++)
        man_parameters[i] = man_inf[i];
    Reset_Data_Man();

    m = M_Count_53;
    m_clothes = m;
    // if_fat=1;  m_cylinder=M_Count_Cylinder_14;
    total_ta = 0;
    j_2 = 0;

    for (i = 0; i < m; i++)
    {
        for (J = 0; J < M_Count_13; J++)
            *(man_parameters[J] + i) = initial_value_man_53[J][i];

        if (mr[i] >= EPS && a[i] >= EPS) ++m_clothes;
        text_man[0][i] = text_man_53[0][i];
        text_man[1][i] = text_man_53[1][i];

        ta[i] = initial_value_env_53[0][i];
        rh[i] = initial_value_env_53[1][i];
        v[i] = initial_value_env_53[2][i];

        if (fabs(ta[i]) >= EPS || rh[i] >= EPS || v[i] >= EPS)                          // <=EPS equivalently ==0 // >=EPS equivalently !=0
        {
            total_ta += ta[i];
            j_2++;
            Penv[i] = 0.1 * exp(18.956 - (4030.18 / (ta[i] + 235))); // kPa;
        }
        else Penv[i] = 0;
    }

    if (j_2 > 0)total_ta /= j_2;

    while (i < M_Compartment_Max)
    {
        ta[i] = 0;
        rh[i] = 0;
        v[i] = 0;
        i++;
    }

    for (i = 1, J = 0; i < m; i++)
    {
        if (mr[i - 1] <= EPS && mr[i] >= EPS && J < M_Count_Cylinder_14)                  // <=EPS equivalently ==0 // >=EPS equivalently !=0
        {
            i_man[0][J] = i;
        }
        if (mr[i] >= EPS && a[i] >= EPS && J < M_Count_Cylinder_14)
        {
            i_man[1][J] = i;
            J++;
        }
    }
    i_torso = 4;
    i_arm = i_torso + 5;

    //Coefficients shivering
    i = 0;
    k_mhp[i++] = 0.86;        //torso
    k_mhp[i++] = 0.06 / 2;      //right arm
    k_mhp[i++] = 0.06 / 2;      //left arm
    k_mhp[i++] = 0 / 2;         //right forearm
    k_mhp[i++] = 0 / 2;         //left forearm
    k_mhp[i++] = 0 / 2;         //right hand
    k_mhp[i++] = 0 / 2;         //left hand
    k_mhp[i++] = 0.08 / 2;      //right thigh
    k_mhp[i++] = 0.08 / 2;      //left thigh
    k_mhp[i++] = 0 / 2;         //right calf
    k_mhp[i++] = 0 / 2;         //left calf
    k_mhp[i++] = 0 / 2;         //right foot
    k_mhp[i] = 0 / 2;         //left foot

    Creation_K_exercise();                                                      // Creation of database of coefficients load (exercise) distribution
    Calc_Man();
    Man_start();                                                                // Calculating sum of man parameters
    Initial_Contr_Resp();                                                       // Setting initial values of thermoregulation parameters
    No_Immersion();
    //No_EMF();

    flag_cloth = 0;

    for (i = 0; i < M_Count_Cylinder_14; i++)
    {
        Cloth_code[i] = 0;
    }
    KPD_air = 0.2;
}
//---------------------------------------------------------------------------

void bioman::Calc_Man(void)
{
    int i;
    sum_ma_man = 0;
    sum_a_man = 0;
    sum_mr_man = 0;
    e_skin_start = 0;
    FLV_start = 0;

    sum_ma_fat = 0;
    sum_ma_skin = 0;
    FLV_sk_start = 0;

    for (i = 0; i < m; i++)
    {
        sum_ma_man += ma[i];
        sum_a_man += a[i];
        sum_mr_man += mr[i];
        FLV_start += flv[i];
    }

    for (i = 0; i < M_Count_Cylinder_14; i++)
    {
        if (i_man[1][i] != 0)
        {                                                                        // <=EPS equivalently ==0 // >=EPS equivalently !=0
            if (i > 0 && mr[i_man[1][i] - 2] >= EPS)
            {
                sum_ma_fat += ma[i_man[1][i] - 1];
            }
            e_skin_start += e[i_man[1][i]];
            sum_ma_skin += ma[i_man[1][i]];
            FLV_sk_start += flv[i_man[1][i]];
        }
    }
    BF = 100 * sum_ma_fat / sum_ma_man;

    r = e[1] + e[i_torso];
    esw = e_skin_start;
    sw = esw / 0.58;

    Muscle_calc();                                //Metabolic rate and blood flow in all skeletal muscles
    Skin_calc();
    Blood_calc();

    tb = flv_mean / sum_flv_man;                      //Initial blood temperature
    Tb_start = tb;                                //Initial blood temperature
    h_r = 20 * tb - 661;                                //Initial heart rate
}
//---------------------------------------------------------------------------

void bioman::Muscle_calc(void)                 //Skeletal Muscles
{
    MR_m_start = 0;
    FLV_m_start = 0;
    sum_ma_muscle = 0;

    for (int i = 1; i < M_Count_Cylinder_14; i++)
    {
        if (i_man[1][i] != 0)
        {                                                                        // <=EPS equivalently ==0 // >=EPS equivalently !=0
            if (mr[i_man[1][i] - 2] >= EPS)
            {
                sum_ma_muscle += ma[i_man[1][i] - 2];
                MR_m_start += mr[i_man[1][i] - 2];                                     //sum heat production in all muscles
                FLV_m_start += flv[i_man[1][i] - 2];
            }
            else
            {
                sum_ma_muscle += ma[i_man[1][i] - 1];
                MR_m_start += mr[i_man[1][i] - 1];
                FLV_m_start += flv[i_man[1][i] - 1];
            }
        }
    }
}
//---------------------------------------------------------------------------

void bioman::Skin_calc(void)
{
    Tsk = 0;
    MR_sk = 0;
    for (int i = 0; i < m; i++)
    {
        if (mr[i] >= EPS && a[i] >= EPS)
        {
            Tsk += te[i] * a[i];               //Mean skin temperature
            MR_sk += mr[i];                  //Skin metabolic rate
        }
    }
    Tsk /= sum_a_man;
}
//---------------------------------------------------------------------------

void bioman::Blood_calc(void)
{
    sum_flv_man = 0;
    flv_mean = 0;

    for (int i = 0; i < m; i++)
    {
        sum_flv_man += flv[i];
        flv_mean += flv[i] * te[i];
    }
}
//---------------------------------------------------------------------------

void bioman::Coef_Skin_Blood_flow_calc(void)              // Calculation of coefficients distribution skin blood flow
{
    int i;
    if (flag_Head_skin_flow)
    {
        if (flag_flv_a) for (i = 0; i < M_Count_Cylinder_14; i++)
        {
            FLS_coef[i] = a_start[i_man[1][i]] / sum_a_man;
        }
        else for (i = 0; i < M_Count_Cylinder_14; i++)
        {
            FLS_coef[i] = flv_start[i_man[1][i]] / FLV_sk_start;
        }
    }
    else
    {
        double without_head_skin_start;
        FLS_coef[0] = 0;

        if (flag_flv_a)
        {
            without_head_skin_start = sum_a_man - a_start[i_man[1][0]];
            for (i = 1; i < M_Count_Cylinder_14; i++)
            {
                FLS_coef[i] = a_start[i_man[1][i]] / without_head_skin_start;
            }
        }
        else
        {
            without_head_skin_start = FLV_sk_start - flv_start[i_man[1][0]];
            for (i = 1; i < M_Count_Cylinder_14; i++)
            {
                FLS_coef[i] = flv_start[i_man[1][i]] / without_head_skin_start;
            }
        }
    }
}
//---------------------------------------------------------------------------

void bioman::Local_BF_skin_calc(void)
{
    int i;
    if (flag_Head_skin_flow)
    {
        for (i = 0; i < M_Count_Cylinder_14; i++)
        {
            flv[i_man[1][i]] = FLS_coef[i] * fls;
            fla[i_man[1][i]] = FLS_coef[i] * fls;
        }
    }
    else
    {
        double flv_without_head_skin;
        double fla_without_head_skin;
        int i_1;
        i_1 = i_man[1][0];
        flv[i_1] = flv_start[i_1];
        fla[i_1] = fla_start[i_1];
        flv_without_head_skin = fls - flv[i_1];
        fla_without_head_skin = fls - fla[i_1];
        for (i = 1; i < M_Count_Cylinder_14; i++)
        {
            flv[i_man[1][i]] = FLS_coef[i] * flv_without_head_skin;
            fla[i_man[1][i]] = FLS_coef[i] * fla_without_head_skin;
        }
    }
}
//---------------------------------------------------------------------------

void bioman::Man_start(void)
{
    double* man_start[M_Count_13] = { mr_start, fla_start, flv_start,
           ka_start, kv_start, e_start, te_start, ma_start,
           a_start, c_start, hc_start, hr_start, k_start };
    int i, j;

    for (i = 0; i < M_Count_13; i++)
        man_parameters_start[i] = man_start[i];

    for (j = 0; j < m; j++)
    {
        for (i = 0; i < M_Count_13; i++)
            *(man_parameters_start[i] + j) = *(man_parameters[i] + j);
    }
    for (; j < M_Compartment_Max; j++)
    {
        for (i = 0; i < M_Count_13; i++)
            *(man_parameters_start[i] + j) = 0;
    }

    Tsk_start = Tsk;                                //Initial value of mean skin temperature
    Tbr_start = te[1];                              //Initial value of brain temperature
    Tb_start = tb;                                  //Initial value of blood temperature
    Tin_org_start = te[4];                          //Initial value of internal organs temperature
}
//---------------------------------------------------------------------------

void bioman::Creation_K_exercise(void)                                            //Creation of database of coefficients load (exercise) distribution
{
    int i, j;
    double k_ext_all_del[7][M_Count_Cylinder_8 - 1] = {
        /*run*/            {0.3, 0.04, 0.04, 0.004, 0.3, 0.3, 0.016},
        /*Bicycling*/      {0.1, 0.05, 0.03, 0.008, 0.5, 0.3, 0.012},
        /*Free*/           {0.3, 0.18, 0.06, 0.008, 0.24, 0.2, 0.012},
        /*Crawl*/          {0.29, 0.18, 0.08, 0.01, 0.22, 0.2, 0.02},
        /*Breast*/         {0.29, 0.16, 0.06, 0.02, 0.28, 0.18, 0.01},
        /*Butterfly*/      {0.3, 0.19, 0.06, 0.015, 0.22, 0.2, 0.015},
        /*Back*/           {0.31, 0.19, 0.05, 0.01, 0.24, 0.19, 0.01} };

    for (j = 0; j < 7; j++) k_ext_all[j][0] = k_ext_all_del[j][0];                       // torso
    k_ext_all[j++][0] = k_ext_all_del[0][0];                                      // torso, current air (run)
    k_ext_all[j][0] = k_ext_all_del[3][0];                                        // torso, current water (crawl)
    for (i = 1; i < M_Count_Cylinder_8 - 1; i++)
    {
        for (j = 0; j < 7; j++)
        {
            k_ext_all[j][2 * i - 1] = 0.5 * k_ext_all_del[j][i];                        // right
            k_ext_all[j][2 * i] = 0.5 * k_ext_all_del[j][i];                          // left
        }

        k_ext_all[j][2 * i - 1] = 0.5 * k_ext_all_del[0][i];                             // current air, right
        k_ext_all[j][2 * i] = 0.5 * k_ext_all_del[0][i];                               // current air, left
        ++j;
        k_ext_all[j][2 * i - 1] = 0.5 * k_ext_all_del[3][i];                           // current water, right
        k_ext_all[j][2 * i] = 0.5 * k_ext_all_del[3][i];                               // current water, left

        if ((2 * i) > M_Count_Cylinder_14 - 1)
        {
            flag_error_b = 1;
            Text_Log_Error_b += "Bioman: Error in Function Creation_K_exercise. ";
            return;
        }
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//----------------------BIOMAN-----------------------------------------------
//---------------------------------------------------------------------------



void bioman::Create_Data(int k)
{
    int i;

    //***___Form_Table_Results___***//
    i = 0;
    Title_total_gr.clear();

    Total_gr[i] = &Time_X;                                                        //0
    Title_total_gr.push_back("Time"); ABR_total_gr.push_back("Time"); Measure_total_gr.push_back("h");
    Total_gr[++i] = &Blood_T;                                                     //1
    Title_total_gr.push_back("Blood temperature"); ABR_total_gr.push_back("T blood"); Measure_total_gr.push_back("°C");
    Total_gr[++i] = &MSkinT;                                                      //2
    Title_total_gr.push_back("Mean skin temperature"); ABR_total_gr.push_back("T skin"); Measure_total_gr.push_back("°C");
    Total_gr[++i] = &MBodyT;                                                      //3
    Title_total_gr.push_back("Mean body temperature"); ABR_total_gr.push_back("T body"); Measure_total_gr.push_back("°C");
    Total_gr[++i] = &MMusclesT;                                                   //4
    Title_total_gr.push_back("Mean muscles temperature"); ABR_total_gr.push_back("T muscles"); Measure_total_gr.push_back("°C");
    Total_gr[++i] = &MFatT;                                                       //5
    Title_total_gr.push_back("Mean fat temperature"); ABR_total_gr.push_back("T fat"); Measure_total_gr.push_back("°C");
    Total_gr[++i] = &Heart_Rate;                                                  //6
    Title_total_gr.push_back("Heart Rate"); ABR_total_gr.push_back("HR"); Measure_total_gr.push_back("beats/min");
    Total_gr[++i] = &Cardiac_output;                                              //7
    Title_total_gr.push_back("Cardiac output"); ABR_total_gr.push_back("CO"); Measure_total_gr.push_back("l/h");
    Total_gr[++i] = &Stroke_volume;                                               //8
    Title_total_gr.push_back("Stroke volume"); ABR_total_gr.push_back("SV"); Measure_total_gr.push_back("mL/stroke");

    Total_gr[++i] = &Total_metabolism;                                            //9
    Title_total_gr.push_back("Total metabolism"); ABR_total_gr.push_back("Metabolism"); Measure_total_gr.push_back("kcal/h");
    Total_gr[++i] = &Metabolism_skin;                                             //10
    Title_total_gr.push_back("Skin metabolism"); ABR_total_gr.push_back("Met. skin"); Measure_total_gr.push_back("kcal/h");
    Total_gr[++i] = &Skin_heat_flow;                                              //11
    Title_total_gr.push_back("Heat flow via blood in skin"); ABR_total_gr.push_back("Heat flow skin"); Measure_total_gr.push_back("kcal/h");
    Total_gr[++i] = &Skin_conduction;                                             //12
    Title_total_gr.push_back("Skin conduction"); ABR_total_gr.push_back("Sk conduct"); Measure_total_gr.push_back("kcal/h");
    Total_gr[++i] = &M_Radiation;                                                 //13
    Title_total_gr.push_back("Radiation"); ABR_total_gr.push_back("Radiation"); Measure_total_gr.push_back("kcal/h");
    Total_gr[++i] = &Air_convection;                                              //14
    Title_total_gr.push_back("Air convection"); ABR_total_gr.push_back("Air conv"); Measure_total_gr.push_back("kcal/h");
    Total_gr[++i] = &Water_convection;                                            //15
    Title_total_gr.push_back("Water convection"); ABR_total_gr.push_back("Water conv"); Measure_total_gr.push_back("kcal/h");
    Total_gr[++i] = &Respiratory_losses;                                          //16
    Title_total_gr.push_back("Respiratory losses"); ABR_total_gr.push_back("Resp los"); Measure_total_gr.push_back("kcal/h");
    Total_gr[++i] = &M_Balance;                                                   //17
    Title_total_gr.push_back("Balance"); ABR_total_gr.push_back("Balance"); Measure_total_gr.push_back("kcal/h");

    Total_gr[++i] = &Required_evaporation;                                        //18
    Title_total_gr.push_back("Required evaporation"); ABR_total_gr.push_back("Req evap"); Measure_total_gr.push_back("kcal/h");
    Total_gr[++i] = &Possible_evaporation;                                        //19
    Title_total_gr.push_back("Possible evaporation"); ABR_total_gr.push_back("Poss evap"); Measure_total_gr.push_back("kcal/h");
    Total_gr[++i] = &Real_evaporation;                                            //20
    Title_total_gr.push_back("Real evaporation"); ABR_total_gr.push_back("Real evap"); Measure_total_gr.push_back("kcal/h");
    Total_gr[++i] = &Dripping_sw_rate;                                            //21
    Title_total_gr.push_back("Dripping sweat rate"); ABR_total_gr.push_back("Dripping"); Measure_total_gr.push_back("g/h");
    Total_gr[++i] = &Evaporation_sw_rate;                                         //22
    Title_total_gr.push_back("Evaporation sweat rate"); ABR_total_gr.push_back("Evap sw rate"); Measure_total_gr.push_back("g/h");
    Total_gr[++i] = &Water_losses;                                                //23
    Title_total_gr.push_back("Water losses"); ABR_total_gr.push_back("WL"); Measure_total_gr.push_back("g");
    Total_gr[++i] = &M_Wettedness;                                                //24
    Title_total_gr.push_back("Wettedness"); ABR_total_gr.push_back("Wettedness"); Measure_total_gr.push_back("");
    Total_gr[++i] = &Sweat_efficiancy;                                            //25
    Title_total_gr.push_back("Sweat efficiancy"); ABR_total_gr.push_back("Sweat eff"); Measure_total_gr.push_back("");
    Total_gr[++i] = &Skin_blood_flow;                                             //26
    Title_total_gr.push_back("Skin blood flow"); ABR_total_gr.push_back("BF skin"); Measure_total_gr.push_back("l/h");
    Total_gr[++i] = &Muscles_blood_flow;                                          //27
    Title_total_gr.push_back("Muscles blood flow"); ABR_total_gr.push_back("BF muscles"); Measure_total_gr.push_back("l/h");
    Total_gr[++i] = &Shivering_total;                                             //28
    Title_total_gr.push_back("Shivering"); ABR_total_gr.push_back("Shivering"); Measure_total_gr.push_back("kcal/h");
    Total_gr[++i] = &Metabolic_rate;                                     //29
    Title_total_gr.push_back("Metabolic rate"); ABR_total_gr.push_back("Metabolic rate"); Measure_total_gr.push_back("kcal/h");

    n_total_gr = i + 1;
    //**********//



    for (i = 0; i < M_Compartment_Max; i++)
    {
        temp[i] = new double[k];
        Evap_sw_rate_loc[i] = new double[k];
        Dripping_loc[i] = new double[k];
        Wettedness_loc[i] = new double[k];
        Blood_flow_loc[i] = new double[k];
        Shivering_loc[i] = new double[k];
        Real_evap_loc[i] = new double[k];
        Possible_evap_loc[i] = new double[k];
        Activity_MET_loc[i] = new double[k];
    }

    for (i = 0; i < n_total_gr; i++)
    {
        *(Total_gr[i]) = new double[k];
    }

    //Time_X =  new double[k];

    Heat_Transmission_M6 = new double[k];

    t_air_gr = new double[k];
    t_water_gr = new double[k];
    Qe_gr = new double[k];
    RiskBodyTemp = new int[k];
    RiskWaterLoses = new int[k];
}
//---------------------------------------------------------------------------

void bioman::Delete_Data(void)
{
    for (int i = 0; i < M_Compartment_Max; i++)
    {
        delete[] temp[i]; temp[i] = NULL;
        delete[] Evap_sw_rate_loc[i]; Evap_sw_rate_loc[i] = NULL;
        delete[] Dripping_loc[i]; Dripping_loc[i] = NULL;
        delete[] Wettedness_loc[i]; Wettedness_loc[i] = NULL;
        delete[] Blood_flow_loc[i]; Blood_flow_loc[i] = NULL;
        delete[] Shivering_loc[i]; Shivering_loc[i] = NULL;
        delete[] Real_evap_loc[i]; Real_evap_loc[i] = NULL;
        delete[] Possible_evap_loc[i]; Possible_evap_loc[i] = NULL;
        delete[] Activity_MET_loc[i]; Activity_MET_loc[i] = NULL;
    }

    for (int i = 0; i < n_total_gr; i++)
    {
        delete[] * (Total_gr[i]); *(Total_gr[i]) = NULL;
    }

    //delete [] Time_X; Time_X=NULL;

    delete[] Heat_Transmission_M6; Heat_Transmission_M6 = NULL;

    delete[] t_air_gr; t_air_gr = NULL;
    delete[] t_water_gr; t_water_gr = NULL;
    delete[] Qe_gr; Qe_gr = NULL;
    delete[] RiskBodyTemp; RiskBodyTemp = NULL;
    delete[] RiskWaterLoses; RiskWaterLoses = NULL;
}
//---------------------------------------------------------------------------

void bioman::SteadyState(void)
{
    Reset_Data_1();                               //obnylenie nekotorix velichin
    Series_Std_St_calc();                         //proverka ysloviya ystanovivshegosya regima
    while (t < 4)
    {
        Stationar();                               //Calculation of local temperature, average temperatures, some heat flows
        t += t_h;
        t_step++;                                  //schetchik dlya vivoda znacheniy na grafiki
    }
    Reset_Data_2();                               //obnulenie raschitivaemix velichin i schetchikov, ustanovka nachalnix znacheniy
}
//---------------------------------------------------------------------------

void bioman::Bio_Change_Run(void)                     //Disturbance    //vozmuschenie
{
    int j_c;
    if (t_step == 0)
    {
        t += t_h;
        t_step++;
        skm = Tsk_start;
    }

    while (t < t_phase + t_h * 0.1)
    {
        if (flag_water > 0 && flag_water < 5) Immersion_add();      //Introduction of immersion
        Exercise_add();       //Introduction of physical activity (load, exercise)
        //Introduction of electromagnetic field
/*if ((t>=emf.t_on_EMF) && (t<emf.t_off_EMF+t_h*0.1) && (!emf.flag_EMF))
{
   emf.flag_EMF=1;
}
if (emf.flag_EMF && t>=emf.t_off_EMF+t_h*0.1)
{
   emf.p_EMF=0;
   for (int i=0; i<m; i++) emf.EM[i]=0;
   emf.flag_EMF=0;
} */

        deri = (Tsk - skm) / t_h;
        dev = Tbr_start - te[1];
        des = Tsk_start - Tsk;
        skm = Tsk;

        //teplovaya regulyaciya
        if (fabs(dev) > h11 || fabs(des) > h12)
            Skin_flows_calc();               //uslovie dlya teplovoy regulyacii  -- izmeneie krovotoka v koje
        Threshold_All_Evapor();          //yslovie poyavleniya regulyatornogo ispareniya i raschet neisparivsheysya vlagi
        if (dev > hdev || des >= hdes)       //cold regulation
        {
            //Function8();
            Skin_flows_calc();
        }
        //if(Qe <= 320*KOEFF)
        Shivering_heat();       //Cold thermogenesis (Shivering) in skeletal muscles

        Physical_evapor_calc();          //Calculation of maximum evaporation from surface of body, based on experimental conditions
        Stationar();                     //Calculation of local temperature, average temperatures, some heat flows
        BuilderGraphAP();                //Forming arrays to display graph

        j_c = i_man[0][1];


        if (te[j_c] > 39.5 || te[j_c] < 35.5)           // Condition to stop the experiment: [core temperature <35.5 or >39.5]
        {
            if (t_phase > t) t_phase = t;
            RiskBodyTemp[t_step] = 3;               // STOP!!!
        }
        else if (te[j_c] > 39 || te[j_c] < 36)          // [core temperature <36 or >39]
        {
            RiskBodyTemp[t_step] = 2;               // Red zone, extreme
        }
        else if (te[j_c] > 38.5 || te[j_c] < 36.3)      // [core temperature <36.3 or >38.5]
        {
            RiskBodyTemp[t_step] = 1;               // Yellow zone, risk
        }
        else                                       // [36.3 < core temperature < 38.5]
        {
            RiskBodyTemp[t_step] = 0;               // Green zone
        }


        if (WL > sum_ma_man * 0.04 * 1000)                // Condition to stop the experiment: [water losses >4% of body weight] (dehydration)
        {
            if (t_phase > t) t_phase = t;
            RiskWaterLoses[t_step] = 3;             // STOP!!!
        }
        else if (WL > sum_ma_man * 0.03 * 1000)           // water losses >3% of body weight (dehydration)
        {
            RiskWaterLoses[t_step] = 2;             // Red zone, extreme
        }
        else if (WL > sum_ma_man * 0.025 * 1000)          // water losses >2.5% of body weight (dehydration)
        {
            RiskWaterLoses[t_step] = 1;             // Yellow zone, risk
        }
        else
        {
            RiskWaterLoses[t_step] = 0;             // Green zone
        }


        if ((t_step > M_Count_Memory - 2) && t_phase > t) t_phase = t;

        t += t_h;
        t_step++;

        if (flag_error_b) break;
    }

    if (t_ex_start > t_phase)t_ex_start = t_phase;
    if (t_ex_finish > t_phase)t_ex_finish = t_phase;
}
//---------------------------------------------------------------------------

void bioman::Exercise_add(void)              //vvedenie fizicheskoy nagruzki
{
    int i, J;

    for (i = 0; i < m; i++) Mwork_loc[i] = 0;

    if (flag_exercise_air == 1 && flag_water != 2 && t >= t_ex_start && (t <= t_ex_finish + t_h * 0.1))
    {
        Qe = Qe_air;
        k_ext = k_ext_air;
        for (i = 0; i < m; i++)
        {
            if (fabs(ta[i]) >= EPS || rh[i] >= EPS || v[i] >= EPS)
            {
                v[i] += v_activity_air;
            }
        }
        total_v += v_activity_air;
        flag_exercise_air = 2;
        Equivalent_he();
    }

    if (flag_exercise_air == 2 && flag_water != 2 && (t >= t_ex_finish + t_h * 0.1))
    {
        Qe = 0;
        ework = 0;
        for (i = 0; i < m; i++)
        {
            if (fabs(ta[i]) >= EPS || rh[i] >= EPS || v[i] >= EPS)
            {
                v[i] -= v_activity_air;
            }
        }
        total_v -= v_activity_air;
        flag_exercise_air = 5;
        Equivalent_he();
    }


    if (Qe > 0)
    {
        J = 0;
        ework = Heat_coef_work * (Qe - sum_mr_man);
        //flm=3*FLV_m_start*ework/75;
        flm = 0;                                                                   //FLV_m_start*ework/sum_mr_man
        for (i = i_torso; i < m; i++)
        {
            if (a[i] >= EPS && mr[i] >= EPS)                                          // <=EPS equivalently ==0 // >=EPS equivalently !=0
            {
                if (mr[i - 2] >= EPS)                                                   // for muscles
                {
                    Mwork_loc[i - 2] = ework * k_ext[J];
                    flv[i - 2] = flv_start[i - 2] + FLV_m_start * k_ext[J] * (Qe - sum_mr_man) / sum_mr_man;
                    flm += flv[i - 2];                                                  // blood flow in muscles during exercise
                    fla[i - 2] = flv[i - 2];
                }
                else                                                               // for muscles
                {
                    Mwork_loc[i - 1] = ework * k_ext[J];
                    flv[i - 1] = flv_start[i - 1] + FLV_m_start * k_ext[J] * (Qe - sum_mr_man) / sum_mr_man;
                    flm += flv[i - 1];                                                  // blood flow in muscles during exercise
                    fla[i - 1] = flv[i - 1];
                }
                J++;
                if (J > M_Count_Cylinder_14 - 1)
                {
                    flag_error_b = 1;
                    Text_Log_Error_b += "Bioman: Error in Function Exercise_add. ";
                    J = M_Count_Cylinder_14 - 1;
                }
            }
        }
    }
    else
    {
        ework = 0;
        flm = FLV_m_start;
        fls = FLV_sk_start;
        for (i = i_torso; i < m; i++)
        {
            if (a[i] >= EPS && mr[i] >= EPS)                                          // <=EPS equivalently ==0 // >=EPS equivalently !=0
            {
                if (mr[i - 2] >= EPS)                                                   // for muscles
                {
                    flv[i - 2] = flv_start[i - 2];
                    fla[i - 2] = fla_start[i - 2];
                }
                else
                {
                    flv[i - 1] = flv_start[i - 1];
                    fla[i - 1] = fla_start[i - 1];
                }
            }
        }
    }
}
//---------------------------------------------------------------------------

void bioman::Immersion_add(void)      //Introduction of immersion
{
    int i, j;

    if (t >= t_on_water && (t <= t_off_water + t_h * 0.1) && flag_water < 2)
    {
        e_skin_start = 0;
        sum_a_man_air = 0;
        for (i = 1; i < m; i++)
        {
            ta_air[i] = ta[i];
            rh_air[i] = rh[i];
            v_air[i] = v[i];

            if (locus_immersion[i] || locus_immersion[i - 1])
            {
                if (hc[i] >= EPS)                                                     // <=EPS equivalently ==0 // >=EPS equivalently !=0
                {
                    hc[i] = hc_water * KOEFF;
                }

                if (mr[i] >= EPS && a[i] >= EPS)
                {
                    e[i] = 0;
                }
                hr[i] = 0;
                if (fabs(ta[i]) >= EPS || rh[i] >= EPS || v[i] >= EPS)
                {
                    ta[i] = temp_water;
                }
            }
            else
            {
                if (mr[i] >= EPS && a[i] >= EPS)
                {
                    sum_a_man_air += a[i];
                    e_skin_start += e_start[i];
                }
                if (fabs(ta[i]) >= EPS || rh[i] >= EPS || v[i] >= EPS)
                {
                    ta[i] = t_air_imm;
                    rh[i] = rh_air_imm;
                    v[i] = v_air_imm;
                }
            }
        }
        ESWW = e_skin_start;

        Qe = Qe_water;
        k_ext = k_ext_water;
        Heat_coef_work = 1 - KPD_water;
        flag_water = 2;

        if (flag_cloth)       // Clohtes
        {
            Man_Clothes();
        }
        Calc_Penv();
        Equivalent_he();
    }
    if (t >= t_off_water + t_h * 0.1 && flag_water < 5)
    {
        e_skin_start = 0;

        for (i = 1; i < m; i++)
        {
            if (locus_immersion[i] || locus_immersion[i - 1])
            {
                e[i] = e_start[i];
                hc[i] = hc_start[i];
                hr[i] = hr_start[i];
            }
            ta[i] = ta_air[i];
            rh[i] = rh_air[i];
            v[i] = v_air[i];
            if (mr[i] >= EPS && a[i] >= EPS)                                  // <=EPS equivalently ==0 // >=EPS equivalently !=0
                e_skin_start += e_start[i];
        }
        ESWW = e_skin_start;
        if (flag_exercise_air == 2)
        {
            Qe = Qe_air;
            k_ext = k_ext_air;
        }
        else
        {
            Qe = 0;
            ework = 0;
        }
        Heat_coef_work = 1 - KPD_air;
        flag_water = 5;

        if (flag_cloth)       // Clohtes
        {
            Man_Clothes();
        }
        Calc_Penv();
        Equivalent_he();
    }
}
//---------------------------------------------------------------------------

void bioman::Threshold_All_Evapor(void)              //yslovie poyavleniya regulyatornogo ispareniya i raschet neisparivsheysya vlagi
{
    //if (te[1]>=h31 || Tsk>=h32 || flag_evaporation)
    if (te[1] >= h31 || flag_evaporation)      //uslovie poyavleniya ispareniya
    {
        Required_evapor_calc();          //Calculation physiologically required (regulatory) evaporation
        Physical_evapor_calc();          //Calculation of maximum evaporation from surface of body, based on experimental conditions
        Real_evaporation_calc();         //Calculation of efficiency and evaluation of evaporation
        Dripping_calc();                    //Calculation of water loss through sweat dripping
        flag_evaporation = 1;
    }
    else                                //raschet proizvodnix velichin, esli net regulyatornogo ispareniya
    {
        if (ESWW < e_skin_start) sw = e_skin_start / 0.58;
        else sw = ESWW / 0.58;

        if (s3 > 0 && ESWW >= e_skin_start) WL = WL + ((ESWW + s3) * t_h / 0.58);
        if (s3 > 0 && ESWW < e_skin_start) WL = WL + ((e_skin_start + s3) * t_h / 0.58);
        if (s3 < 0) WL = WL + (e_skin_start * t_h / 0.58);
        wet = 0.03;
        for (int i = 0; i < m; i++) we[i] = 0.03;
    }
}
//---------------------------------------------------------------------------

void bioman::Real_evaporation_calc(void)  //Calculation of real evaporation (total/local) and derivative values
{
    //Calculation of efficiency and evaluation of evaporation
    if (esw < ESWmax)
    {
        ESWW = esw;            //real = required
        //raschet uvlajnennosti poverxnosti koji
        if (ESWmax >= EPS) wet = ESWW / ESWmax;  //Wettedness                         // <=EPS equivalently ==0 // >=EPS equivalently !=0
        //raschet effektivnosti i ocenki ispareniya
        Sw_eff = 1 - (wet * wet / 2);

        SWdrip = 0;
    }
    else
    {            //real = 2*physical
        ESWW = ESWmax;
        SWdrip = (esw - ESWmax) / 0.58;
        wet = 1;
        Sw_eff = 0.5;
    }
    Real_evap_local();

    //raschet skorosti potootdeleniya
    sw = ESWW / 0.58;
    WL = WL + (esw * t_h / 0.58);
}
//---------------------------------------------------------------------------

void bioman::Dripping_calc(void)          //Calculation of water loss through sweat dripping
{
    for (int i = 1; i < m; i++)
    {
        if (esw < ESWmax)
        {
            if (wet >= kw) SWdrip_loc[i] = (ESWW_loc[i] - esw_loc[i]) / 0.58;
            else   SWdrip_loc[i] = 0;
        }
        else SWdrip_loc[i] = esw_loc[i] / 0.58;
    }
}
//---------------------------------------------------------------------------

void bioman::Required_evapor_calc(void)   //Calculation physiologically required (regulatory) evaporation
{
    double sk_evap;

    if (Tsk >= Tsk_start) esw = e_skin_start - s31 * (Tbr_start - te[1]) - s32 * (Tsk_start - Tsk);
    else esw = e_skin_start - s31 * (Tbr_start - te[1]) - s32 * (Tsk_start - Tsk) - s34 * (sqn - sq) - s33 * deri;
    if (esw < e_skin_start)
    {
        esw = e_skin_start;
    }

    sk_evap = sum_a_man;

    //Calculation of local values
    for (int i = 1; i < m; i++)
    {                                                                           // <=EPS equivalently ==0 // >=EPS equivalently !=0
        if (a[i] >= EPS && mr[i] >= EPS)
        {
            esw_loc[i] = a[i] * esw / sk_evap;
            e[i] = a[i] * esw / sk_evap;
        }
    }
}
//---------------------------------------------------------------------------

void bioman::Physical_evapor_calc(void)   //Calculation of maximum evaporation from surface of body, based on experimental conditions
{
    ESWmax = 0;
    for (int i = 1; i < m; i++)   //clothes
    {
        ESWm[i] = e[i];
        if (thickness[i] >= EPS)  //variants of clothes
        {
            //proekt "ISO"
            if (flag_water == 2 && (locus_immersion[i] || locus_immersion[i - 1]))
                ESWm[i - 1] = 0;
            else
            {
                ESWm[i - 1] = heeq[i] * a[i - 1] * kt * (Pa[i - 1] - rh[i] * Penv[i]);

                if (Aclo_pct[i] < 1)
                {
                    ESWm[i - 1] = heeq[i] * Aclo_pct[i] * a[i - 1] * kt * (Pa[i - 1] - rh[i] * Penv[i]);
                    ESWm[i - 1] += heeq[i - 1] * (1 - Aclo_pct[i]) * a[i - 1] * (Pa[i - 1] - rh[i] * Penv[i]);
                }
            }
        }
    }

    for (int i = 1; i < m; i++)
    {                                                                           // <=EPS equivalently ==0 // >=EPS equivalently !=0
        if (a[i] >= EPS)
        {
            if (m != m_clothes)                                              // Naked (nude)
                ESWm[i] = heeq[i] * a[i] * (Pa[i] - rh[i] * Penv[i]);
            else if (mr[i] >= EPS && thickness[i + 1] <= EPS)                    //danniy uchastok clothes
                ESWm[i] = heeq[i] * a[i] * (Pa[i] - rh[i] * Penv[i]);
            if (flag_water == 2 && (locus_immersion[i] || locus_immersion[i - 1]))
                ESWm[i] = 0;
        }
        if (ESWm[i] < 0)
            ESWm[i] = 0;

        if (a[i] >= EPS && mr[i] >= EPS)
        {
            ESWmax += ESWm[i];           //Calculation of total maximum evaporation from surface of clothes, based on local values
        }
    }
}
//---------------------------------------------------------------------------

void bioman::Real_evap_local(void)
{
    double sk_evap;
    if (flag_water == 2) sk_evap = sum_a_man_air;
    else sk_evap = sum_a_man;

    //Calculation of local real evaporation
    for (int i = 1; i < m; i++)
    {
        //CLOTHES*************************
        if (thickness[i] >= EPS) //clothes
        {
            if (flag_water == 2 && (locus_immersion[i] || locus_immersion[i - 1]))
                ESWW_loc[i - 1] = 0;
            else
                ESWW_loc[i - 1] = a[i - 1] * ESWW / sk_evap;
            if (ESWm[i - 1] != 0) we[i - 1] = ESWW_loc[i - 1] / ESWm[i - 1];
            else we[i - 1] = 0;
            e[i - 1] = ESWW_loc[i - 1];
        }
        //END CLOTHES*************************
        //Naked (nude)*************************
        if (mr[i] >= EPS && a[i] >= EPS && thickness[i + 1] <= EPS)//skin    // <=EPS equivalently ==0 // >=EPS equivalently !=0
        {
            if (flag_water == 2 && (locus_immersion[i] || locus_immersion[i - 1]))
                ESWW_loc[i] = 0;
            else
                ESWW_loc[i] = a[i] * ESWW / sk_evap;
            if (ESWm[i] >= EPS) we[i] = ESWW_loc[i] / ESWm[i];
            else we[i] = 0;
            e[i] = ESWW_loc[i];

        }
        //END Naked (nude)**************************************
    }
}
//---------------------------------------------------------------------------

void bioman::Skin_flows_calc(void)              // Heat skin flows
{
    double am = 1, ah = 1;

    if (fabs(dev) > h11 && fabs(des) > h12)
    {
        am = am1;
        ah = ah1;
    }
    if (fabs(dev) > h11 && fabs(des) < h12)
    {
        am = am2;
        ah = ah2;
    }
    if (fabs(dev) < h11 && fabs(des) > h12)
    {
        am = am3;
        ah = ah3;
    }
    if (fabs(dev) < h11 && fabs(des) < h12)
    {
        am = am4;
        ah = ah4;
    }
    fls = FLV_sk_start - am * (Tbr_start - te[1]) - ah * (Tsk_start - Tsk) + s13 * deri - s14 * (sqn - sq);
    if (fls > fls_max) fls = fls_max;                                               // Maximum skin blood flow 6-8 l/min (360-480 l/h)
    if (fls < fls_min) fls = fls_min;                                               // Minimum skin blood flow
    //regionalnoe raspredelenie kojnogo krovotoka
    Local_BF_skin_calc();
}
//---------------------------------------------------------------------------

void bioman::Restriction_Tcl(void)
{
    for (int i = 1; i < m - 1; i++)
    {                                                                           // <=EPS equivalently ==0 // >=EPS equivalently !=0
        if (thickness[i] >= EPS)
        {
            if (te[i - 1] > ta[i])
            {
                if (te[i] < ta[i])
                    te[i] = ta[i];
                if (te[i] > te[i - 1])
                    te[i] = te[i - 1];
            }
            else
            {
                if (te[i] > ta[i])
                    te[i] = ta[i];
                if (te[i] < te[i - 1])
                    te[i] = te[i - 1];
            }
            //te[i]=(te[i-1]+ta[i])/2;
        }
    }
}
//---------------------------------------------------------------------------

void bioman::Stationar(void)              //Calculation of local temperature, average temperatures, some heat flows
{
    int i;
    double der1[M_Compartment_Max], der2[M_Compartment_Max], der3[M_Compartment_Max];
    double tem[M_Compartment_Max], der[M_Compartment_Max];

    for (i = 1; i < m; i++)
    {
        tem[i] = 0;
        der[i] = 0;  der1[i] = 0; der2[i] = 0; der3[i] = 0;
        Pa[i] = 0;
        // <=EPS equivalently ==0 // >=EPS equivalently !=0
        if (a[i] >= EPS && te[i] > -235)
            Pa[i] = 0.1 * exp(18.956 - (4030.18 / (te[i] + 235)));                      //raschet davleniya nasischennix parov pri opredelennoy temperature kompartmenta

        //izmenenie local temperature za vremya integrirovaniya

        if (mr[i] >= EPS)                                                      // for core, muscle, fat, skin
            der1[i] = mr[i] + Mwork_loc[i] + Msh_loc[i] +
            ro * ce * ka[i] * fla[i] * tb -
            ro * ce * kv[i] * flv[i] * te[i];

        // qhk -- earlier k -- thermal conductivity between layers - теплопроводность между слоями
                                                                                 // <=EPS equivalently ==0 // >=EPS equivalently !=0
        if (mr[i] >= EPS && mr[i - 1] >= EPS)                                  // for muscle, fat, skin
            der2[i] = -qhk[i] * (te[i] - te[i - 1]);                        // теплопередача кондукцией между слоями
        if (mr[i] >= EPS && a[i] <= EPS)                                      // for core, muscle, fat.
            der2[i] += -qhk[i + 1] * (te[i] - te[i + 1]);                     // теплопередача кондукцией между слоями
        if (thickness[i + 1] >= EPS)                                              // for "clothed" skin.
            der2[i] += -qhk[i + 1] * (te[i] - te[i + 1]);                     // передача тепла кондукцией от кожи к одежде
        // qhk and qh_clo for clothes is calculated in function Equivalent_he()
        if (thickness[i] >= EPS)                                                // for clothes
            der2[i] = -qhk[i] * (te[i] - te[i - 1]);                        //-qh_clo[i]*(te[i]-ta[i]);

        /*if (thickness[i]<=EPS) der2[i] = -qhk[i]*(te[i]-te[i-1])-qhk[i+1]*(te[i]-te[i+1]);
        else der2[i] = -qhk[i]*(te[i]-te[i-1])-qhk[i+1]*(te[i]-ta[i]);   */

        if (flag_water == 2 && (locus_immersion[i] || locus_immersion[i - 1]))   //water
        {                                                                        // <=EPS equivalently ==0 // >=EPS equivalently !=0
            if (a[i] >= EPS && thickness[i] <= EPS)
            {
                if (thickness[i + 1] <= EPS)                                            // for "naked (nude)" skin in water
                    der3[i] = -a[i] * hc[i] * (te[i] - ta[i]);
                else                                                               // partially clothed skin in water
                    der3[i] = (1 - Aclo_pct[i + 1]) * (-a[i] * hc[i] * (te[i] - ta[i + 1]));
            }
            if (thickness[i] >= EPS)                                                 // for clothes
            {
                //der3[i] = -a[i]*hc[i]*(te[i]-ta[i]);                             // convective-conductive heat transfer from clothes in water
                der3[i] = -qh_clo[i] * (te[i] - ta[i]);                                // heat transfer from clothes in water
            }
        }
        else                                                                     // air
        {
            if (mr[i] >= EPS) der3[i] = -e[i];                                         // <=EPS equivalently ==0 // >=EPS equivalently !=0

            if (mr[i] >= EPS && a[i] >= EPS)                                           // for skin in air
            {
                if (thickness[i + 1] <= EPS)                                            // for "naked (nude)" skin
                    der3[i] += -a[i] * hr[i] * (te[i] - ta[i]) -
                    a[i] * hc[i] * sqrt(10 * v[i]) * (te[i] - ta[i]);
                else                                                               // partially clothed skin in air
                    der3[i] += (1 - Aclo_pct[i + 1]) * (-a[i] * hr[i] * (te[i] - ta[i + 1]) -
                        a[i] * hc[i] * sqrt(10 * v[i + 1]) * (te[i] - ta[i + 1]));
            }
            if (thickness[i] >= EPS)                                                 // for clothes
                der3[i] = -qh_clo[i] * (te[i] - ta[i]);                                // heat transfer from clothes in air

            /*if (mr[i]>=EPS && m!=m_clothes && emf.flag_EMF && emf.p_EMF>0)      // EMF (only for naked man)
               der3[i] += emf.EM[i];*/
        }
        // <=EPS equivalently ==0 // >=EPS equivalently !=0
        if (mr[i] >= EPS || thickness[i] >= EPS)                                     //izmenenie local temperature za vremya integrirovaniya
        {
            if (c[i] <= EPS) der[i] = (der1[i] + der2[i] + der3[i]) / (0.83 * ma[i]);
            else der[i] = (der1[i] + der2[i] + der3[i]) / (c[i] * ma[i]);

            tem[i] = te[i] + der[i] * t_h;                                              //Calculation of current local temperature
        }
    }
    for (i = 1; i < m; i++) te[i] = tem[i];
    if (m == m_clothes) Restriction_Tcl();                                        //температура одежды не может выходить за пределы диапазона [температура среды, температура кожи]

    Skin_calc();                                                                //Calculation of mean skin temperature
    Blood_calc();                                                               //Calculation of blood temperature, cardiac output, heart rate
    Calc1();                                                                    //Calculation of heat flow, average temperature
    convergence = sum_mr_man - e_skin_start - r - sq;

    //teplovoy balans
    if (fabs(esw - e_skin_start) >= EPS) s3 = MR_total - ESWW - r - sq;         // <=EPS equivalently ==0 // >=EPS equivalently !=0
    else s3 = MR_total - esw - r - sq;
}
//---------------------------------------------------------------------------

void bioman::Series_Std_St_calc(void)    //proverka usloviya ustanovivshegosya rejima
{
    int P;                              //number of integration steps
    double precision;
    P = 0;
    while (P < 5)
    {
        precision = 0.001;
        convergence = 1;
        while (fabs(convergence) > precision && t_step < M_Count_Memory - 2)            //cikl, poka ne dostignuya zadannaya tochnost
        {
            t += t_h;
            t_step++;
            Stationar();                                //Calculation of local temperature, average temperatures, some heat flows
        }
        t = 0;
        t_step = 0;
        if (fabs(convergence) <= precision) ++P;
    }

    if (fabs(convergence) <= precision) flag_Recalc = 1;
}
//---------------------------------------------------------------------------

void bioman::Reset_Data_1(void)
{
    t_h = 0.001;
    t = 0;
    t_step = 0;
    sqn4 = sqn3 = sqn = smn = 0;

    for (int i = 1; i < m - 1; i++)
    {
        if (a[i] >= EPS)                                                           // <=EPS equivalently ==0 // >=EPS equivalently !=0
        {
            sqn4 += a[i] * hc[i] * (te[i] - ta[i]);
            sqn3 += a[i] * hr[i] * (te[i] - ta[i]);
            sqn += a[i] * (hr[i] + hc[i]) * (te[i] - ta[i]);
            smn += qhk[i] * (te[i - 1] - te[i]);
        }
    }

    for (int i = 0; i < M_Compartment_Max; i++)
    {
        Mwork_loc[i] = 0;
        Msh_loc[i] = 0;
        Pa[i] = 0;
        Penv[i] = 0;
    }

    Qe = 0;
    ework = 0;
    flag_evaporation = 0;
    t_cold_shiver = 0;
    T_sh_max = 100;
    Msh = 0;
    Sw_eff = 1;
}
//---------------------------------------------------------------------------

void bioman::Reset_Data_2(void)
{
    //obnulenie raschitivaemix velichin i schetchikov, ustanovka nachalnix znacheniy
    t_step = 0;
    i_color = 0;
    t = 0;

    for (int j = 0; j < M_Count_Memory; j++)
    {
        for (int i = 0; i < M_Compartment_Max; i++) temp[i][j] = 0;
        Time_X[j] = 0;
    }

    for (int i = 0; i < M_Compartment_Max; i++)
    {
        ESWW_loc[i] = 0;
        SWdrip_loc[i] = 0;
        esw_loc[i] = e[i];
        ESWm[i] = e[i];
    }

    fls = FLV_sk_start;
    ESWW = esw;
    sw = ESWW / 0.58;
    WL = e_skin_start / 0.58;
    ESWmax = 8.49;
    SWdrip = 0;
    wet = 0;
    flm = FLV_m_start;
}
//---------------------------------------------------------------------------

void bioman::Equivalent_he(void)
{
    //raschet obobschennogo koefficienta teplootdachi ispareniem v sredu dlya
    //cheloveka bez odejdi i v odejde,obobschennogo koefficienta teploobmana
    //mejdu kojey i vnutrennim sloem odejdi, vneshnim sloem odejdi i sredoy dlya
    //odetogo cheloveka
    double Rair[M_Compartment_Max];
    double Re[M_Compartment_Max], Reeq[M_Compartment_Max];
    double hceq[M_Compartment_Max], Renv[M_Compartment_Max];
    double Lamda, dair;
    bool flag_water_cl;
    Lamda = 0.026;                        //coefficient of thermal conductivity of air (300 K, 100 kPa)
    dair = 0.002;                         //thickness of air layer between clothes and skin
    Tot_R = 0;
    for (int i = 0; i < M_Compartment_Max; i++)
    {
        Reeq[i] = Re[i] = Rair[i] = Renv[i] = hceq[i] = heeq[i] = 0;
    }
    for (int i = 0; i < m - 1; i++)
    {
        if (flag_water == 2 && (locus_immersion[i] || locus_immersion[i - 1]))
        {
            flag_water_cl = true;
        }
        else flag_water_cl = false;

        if (!flag_water_cl && Ref[i] < 1)
        {
            hceq[i] = hc[i] * sqrt(10 * v[i]);                                         // Calculation coefficient of air convection depending of air velocity
            if (thickness[i + 1] >= EPS && Aclo_pct[i + 1] < 1)                            // clothed skin or partially clothed skin
                hceq[i] = hc[i] * sqrt(10 * v[i + 1]);
        }
        if (thickness[i] >= EPS)                                // Clothes    // <=EPS equivalently ==0 // >=EPS equivalently !=0
        {
            if (flag_water_cl)
            {
                qhk[i] = a[i] / Rf[i];                      //теплопроводность между кожей и одеждой
                Renv[i] = hc[i];                                  // коэффициент конвективной теплопередачи в воде
                if (Renv[i] >= EPS) Renv[i] = 1 / Renv[i];

                qh_clo[i] = a[i] / (0.5 * Rf[i] + Renv[i]);     //передача тепла от одежды в воду
                // qh_clo для воды -> конвективно-кондуктивная передача тепла од одежды в воду
            }
            else
            {
                Rair[i] = 1 / (hr[i] + hceq[i] + (Lamda * KOEFF / dair));
                qhk[i] = a[i] / (Rf[i] + (Rair[i] / (1 + FCL * Rf[i])));        //теплопроводность между кожей и одеждой через воздух
                Tot_R += (a[i] * Rair[i]);
                Renv[i] = hr[i] + hceq[i];
                if (Renv[i] >= EPS) Renv[i] = 1 / Renv[i];
                qh_clo[i] = a[i] / (0.5 * Rf[i] + (Renv[i] / (1 + FCL * Rf[i]))); //передача тепла от одежды в воздух
                // qh_clo -- передача тепла от одежды в среду
            }
            //qh_clo[i]=a[i]*(1+FCL*Rf[i])/Renv[i];
            //if(flag_water_cl) qh_clo[i]=a[i]/(Rf[i]+Renv[i]);
            //qh_clo[i]=a[i]/(0.5*Rf[i]+(Renv[i]/(1+FCL*Rf[i])));  //передача тепла от одежды в среду
        }

        if (hceq[i] >= EPS) Re[i] = 1 / (16.5 * hceq[i] * (1 + FCL * Rf[i]));              //raschet ekvivalentnogo vozdushnogo soprotivleniya
        if (ip[i] <= EPS) Reeq[i] = Re[i];                                       //raschet ekvivalentnogo soprotivleniya odejdi ispareniyu
        else Reeq[i] = Ref[i] + Re[i];

        if (Reeq[i] >= EPS) heeq[i] = 1 / Reeq[i];
    }
}
//---------------------------------------------------------------------------

/*void bioman::Function8(void)   //Cold skin flows
{
   // DON'T WORK!!! Function8 -- ERROR!!!

   if ( (fabs(dev)>hdev && dev>0)|| (fabs(des)>hdes && des>0))
   {
      //fls=FLV_sk_start-s22*(Tsk_start-Tsk)-s24*(sqn-sq)-s21*(Tbr_start-te[1]);
      //fls=FLV_sk_start;
      //svereno s NFIZ
      if (fls<1) fls=1;
      if (fls>300) fls=300;
      //regionalnoe raspredelenie kojnogo krovotoka
      for (int i=1; i<m-1; i++)
      {                                                                        // <=EPS equivalently ==0 // >=EPS equivalently !=0
         if (a[i]>=EPS && mr[i]>=EPS)
         {
            flv[i]=flv_start[i]-fls*a[i]/sum_a_man;
            fla[i]=fla_start[i]-fls*a[i]/sum_a_man;
         }
      }
   }
}  */
//---------------------------------------------------------------------------

void bioman::Shivering_heat(void)              //Cold thermogenesis (Shivering) in skeletal muscles
{
    int J;
    double Msh_max;                         //The maximal shivering intensity [kcal/h]

    Msh_max = 350;                           //Maximum metabolic heat 5*sum_mr_man
    Msh = 0;
    for (int i = 0; i < M_Compartment_Max; i++) Msh_loc[i] = 0;

    if (t_cold_shiver < T_sh_max && (des > h42 || dev > h41))
    {
        //Msh=KOEFF*sum_a_man*(155.5*(Tbr_start-te[1])+47.0*(Tsk_start-Tsk)-1.57*(Tsk_start-Tsk)*(Tsk_start-Tsk))/sqrt(BF); //modern (Wissler)
        if (flag_water == 2)                     //Muscle thermogenesis during water immersion
        {
            if (des < 0) Msh = KOEFF * sum_a_man * (155.5 * (Tbr_start - te[1]) + 47.0 * (Tsk_start - Tsk) - 1.57 * (Tsk_start - Tsk) * (Tsk_start - Tsk)) / sqrt(BF); //modern (Wissler)
            else Msh = KOEFF * sum_a_man * (5 * (Tsk_start - Tsk) * (Tbr_start - te[1]) + 65 * pow((Tsk_start - Tsk) / BF, 1.5));
            //if(imm.temp_water<=10) Msh=...;
        }
        else                                  //Muscle thermogenesis on air environment
        {
            if ((des > h42) && (dev > h41))
            {
                Msh = KOEFF * sum_a_man * (155.5 * (Tbr_start - te[1]) + 47.0 * (Tsk_start - Tsk) - 1.57 * (Tsk_start - Tsk) * (Tsk_start - Tsk)) / sqrt(BF); //modern (Wissler)
            }
        }

        if (Msh < 0)Msh = 0;
        if (Msh > Msh_max)
            Msh = Msh_max;     //kcal/h. Maximum metabolic heat 5*sum_mr_man
        flm = 0;
        J = 0;

        for (int i = i_torso; i < m; i++)
        {
            if (a[i] >= EPS && mr[i] >= EPS)                                          // <=EPS equivalently ==0 // >=EPS equivalently !=0
            {
                //Local shivering
                if (mr[i - 2] >= EPS)
                {
                    Msh_loc[i - 2] = Msh * k_mhp[J];                                      // for muscles
                    //flv[i-2]+=flv_start[i-2]*Msh/sum_mr_man*k_mhp[J];
                    if (k_mhp[J] >= EPS) flv[i - 2] += flv_start[i - 2] * Msh / sum_mr_man;
                    fla[i - 2] = flv[i - 2];
                    flm += flv[i - 2];
                }
                else
                {
                    Msh_loc[i - 1] = Msh * k_mhp[J];                                      // for muscles
                    //flv[i-1]+=flv_start[i-1]*Msh/sum_mr_man*k_mhp[J];
                    if (k_mhp[J] >= EPS) flv[i - 1] += flv_start[i - 1] * Msh / sum_mr_man;
                    fla[i - 1] = flv[i - 1];
                    flm += flv[i - 1];
                }
                J++;
            }
        }
    }


    //*** hc(v) ***
    /*if(flag_water==2 && Form_Experiment->MaskEdit_hc->Text=="1")
    {
       double v_activity, hc_t;
       v_activity=(double)Qe/(KOEFF*0.002*sum_ma_man*3600);                     // m/s
       hc_t=0;
       if(Msh<=EPS)
       {
          if(v_activity<0.06) hc_t=415.4*v_activity+41.4;                       // W / (m^2 * °C)
          else hc_t=272.9*sqrt(v_activity);
       }
       else
       {
          if(v_activity<0.11) hc_t=590*v_activity+53.1;
          else hc_t=497.1*pow(v_activity, 0.65);
       }

       hc_t*=KOEFF;                                                             // kcal / (m^2 * °C * h)
       for(int i=1; i<m; i++)
          if(locus_immersion[i] || locus_immersion[i-1])
             if(hc[i]>=EPS) hc[i]=hc_t;
    }*/
    //*** _ ***
}
//---------------------------------------------------------------------------

void bioman::Clone_Parameters(void)
{
    int i, j, J;
    double ta_clone[M_Compartment_Max], rh_clone[M_Compartment_Max],
        v_clone[M_Compartment_Max], Penv_clone[M_Compartment_Max],
        ta_air_clone[M_Compartment_Max], rh_air_clone[M_Compartment_Max],
        v_air_clone[M_Compartment_Max], Pa_clone[M_Compartment_Max];

    //double EM_clone[M_Compartment_Max];

    double Ref_clone[M_Compartment_Max], Rf_clone[M_Compartment_Max],
        thickness_clone[M_Compartment_Max], ip_clone[M_Compartment_Max];

    double man_clone[M_Count_13][M_Compartment_Max];
    double man_start_clone[M_Count_13][M_Compartment_Max];
    string text_man_clone[2][M_Compartment_Max];

    double esw_loc_clone[M_Compartment_Max];

    double* temp_clone[M_Compartment_Max];
    double* Evap_sw_rate_loc_clone[M_Compartment_Max];
    double* Dripping_loc_clone[M_Compartment_Max];
    double* Wettedness_loc_clone[M_Compartment_Max];
    double* Blood_flow_loc_clone[M_Compartment_Max];
    double* Shivering_loc_clone[M_Compartment_Max];
    double* Real_evap_loc_clone[M_Compartment_Max];
    double* Possible_evap_loc_clone[M_Compartment_Max];
    double* Activity_MET_loc_clone[M_Compartment_Max];

    m = m_clothes;
    i_torso += 1;
    i_arm += 2;

    for (i = 0; i < m; i++)
    {
        // Clone_Environment_Parameters
        ta_clone[i] = ta[i]; ta[i] = 0;
        rh_clone[i] = rh[i]; rh[i] = 0;
        v_clone[i] = v[i]; v[i] = 0;
        Penv_clone[i] = Penv[i]; Penv[i] = 0;
        ta_air_clone[i] = ta_air[i]; ta_air[i] = 0;
        rh_air_clone[i] = rh_air[i]; rh_air[i] = 0;
        v_air_clone[i] = v_air[i]; v_air[i] = 0;
        Pa_clone[i] = Pa[i]; Pa[i] = 0;

        // Clone_Man_Parameters
        for (J = 0; J < M_Count_13; J++)
        {
            man_clone[J][i] = *(man_parameters[J] + i);
            *(man_parameters[J] + i) = 0;
            man_start_clone[J][i] = *(man_parameters_start[J] + i);
            *(man_parameters_start[J] + i) = 0;
        }
        text_man_clone[0][i] = text_man[0][i]; text_man[0][i].clear();
        text_man_clone[1][i] = text_man[1][i]; text_man[1][i].clear();

        // Clone_Evaporation
        esw_loc_clone[i] = esw_loc[i]; esw_loc[i] = 0;

        // Clone_clothes_Parameters
        Ref_clone[i] = Ref[i]; Ref[i] = 0;
        Rf_clone[i] = Rf[i]; Rf[i] = 0;
        thickness_clone[i] = thickness[i]; thickness[i] = 0;
        ip_clone[i] = ip[i]; ip[i] = 0;

        // Clone_EMF_Parameters
        //EM_clone[i] = emf.EM[i]; emf.EM[i]=0;

        //Clone_local_graph
        temp_clone[i] = new double[t_step + 2];
        Evap_sw_rate_loc_clone[i] = new double[t_step + 2];
        Dripping_loc_clone[i] = new double[t_step + 2];
        Wettedness_loc_clone[i] = new double[t_step + 2];
        Blood_flow_loc_clone[i] = new double[t_step + 2];
        Shivering_loc_clone[i] = new double[t_step + 2];
        Real_evap_loc_clone[i] = new double[t_step + 2];
        Possible_evap_loc_clone[i] = new double[t_step + 2];
        Activity_MET_loc_clone[i] = new double[t_step + 2];
        for (J = 0; J < t_step + 1; J++)
        {
            temp_clone[i][J] = temp[i][J]; temp[i][J] = 0;
            Evap_sw_rate_loc_clone[i][J] = Evap_sw_rate_loc[i][J]; Evap_sw_rate_loc[i][J] = 0;
            Dripping_loc_clone[i][J] = Dripping_loc[i][J]; Dripping_loc[i][J] = 0;
            Wettedness_loc_clone[i][J] = Wettedness_loc[i][J]; Wettedness_loc[i][J] = 0;
            Blood_flow_loc_clone[i][J] = Blood_flow_loc[i][J]; Blood_flow_loc[i][J] = 0;
            Shivering_loc_clone[i][J] = Shivering_loc[i][J]; Shivering_loc[i][J] = 0;
            Real_evap_loc_clone[i][J] = Real_evap_loc[i][J]; Real_evap_loc[i][J] = 0;
            Possible_evap_loc_clone[i][J] = Possible_evap_loc[i][J]; Possible_evap_loc[i][J] = 0;
            Activity_MET_loc_clone[i][J] = Activity_MET_loc[i][J]; Activity_MET_loc[i][J] = 0;
        }
    }

    for (i = 0, j = 0; i < m; i++)
    {
        // Clone_Environment_Parameters
        ta[i] = ta_clone[j];
        rh[i] = rh_clone[j];
        v[i] = v_clone[j];
        Penv[i] = Penv_clone[j];
        ta_air[i] = ta_air_clone[j];
        rh_air[i] = rh_air_clone[j];
        v_air[i] = v_air_clone[j];
        Pa[i] = Pa_clone[j];

        // Clone_Man_Parameters
        for (J = 0; J < M_Count_13; J++)
        {
            *(man_parameters[J] + i) = man_clone[J][j];
            *(man_parameters_start[J] + i) = man_start_clone[J][j];
        }
        text_man[0][i] = text_man_clone[0][j];
        text_man[1][i] = text_man_clone[1][j];

        // Clone_Evaporation
        esw_loc[i] = esw_loc_clone[j];

        // Clone_clothes_Parameters
        Ref[i] = Ref_clone[j];
        Rf[i] = Rf_clone[j];
        thickness[i] = thickness_clone[j];
        ip[i] = ip_clone[j];

        // Clone_EMF_Parameters
        //emf.EM[i] = EM_clone[j];

        //Clone_local_graph
        for (J = 0; J < t_step + 1; J++)
        {
            temp[i][J] = temp_clone[j][J];
            Evap_sw_rate_loc[i][J] = Evap_sw_rate_loc_clone[j][J];
            Dripping_loc[i][J] = Dripping_loc_clone[j][J];
            Wettedness_loc[i][J] = Wettedness_loc_clone[j][J];
            Blood_flow_loc[i][J] = Blood_flow_loc_clone[j][J];
            Shivering_loc[i][J] = Shivering_loc_clone[j][J];
            Real_evap_loc[i][J] = Real_evap_loc_clone[j][J];
            Possible_evap_loc[i][J] = Possible_evap_loc_clone[j][J];
            Activity_MET_loc[i][J] = Activity_MET_loc_clone[j][J];
        }
        delete[] temp_clone[j]; temp_clone[j] = NULL;
        delete[] Evap_sw_rate_loc_clone[j]; Evap_sw_rate_loc_clone[j] = NULL;
        delete[] Dripping_loc_clone[j]; Dripping_loc_clone[j] = NULL;
        delete[] Wettedness_loc_clone[j]; Wettedness_loc_clone[j] = NULL;
        delete[] Blood_flow_loc_clone[j]; Blood_flow_loc_clone[j] = NULL;
        delete[] Shivering_loc_clone[j]; Shivering_loc_clone[j] = NULL;
        delete[] Real_evap_loc_clone[j]; Real_evap_loc_clone[j] = NULL;
        delete[] Possible_evap_loc_clone[j]; Possible_evap_loc_clone[j] = NULL;
        delete[] Activity_MET_loc_clone[j]; Activity_MET_loc_clone[j] = NULL;

        j++;
        if (a[i] >= EPS && mr[i] >= EPS)                                             // <=EPS equivalently ==0 // >=EPS equivalently !=0
            i++;
    }

    for (i = 1, J = 0; i < m; i++)
    {                                                                           // <=EPS equivalently ==0 // >=EPS equivalently !=0
        if (mr[i - 1] <= EPS && mr[i] >= EPS && J < M_Count_Cylinder_14)
        {
            i_man[0][J] = i;
        }
        if (a[i] >= EPS && mr[i] >= EPS && J < M_Count_Cylinder_14)
        {
            i_man[1][J] = i;
            J++;
        }
    }
}
//---------------------------------------------------------------------------

void bioman::Man_Clothes(void)
{
    int i, j;

    No_Clothes();

    for (j = 0; j < M_Count_Cylinder_14; j++)
    {
        i = i_man[1][j];

        if (thickness_a[j] >= EPS || thickness_w[j] >= EPS)
        {                                                                        // <=EPS equivalently ==0 // >=EPS equivalently !=0
            if (text_man[1][i + 1].empty())                                          //==NULL
            {
                te[i + 1] = ta[i];                                                   //zadanie nachalnoy temperaturi odejdi
                //te[i+1]=(te[i]+ta[i])/2;

                ta[i + 1] = ta[i];
                ta[i] = 0;
                rh[i + 1] = rh[i];
                rh[i] = 0;
                v[i + 1] = v[i];
                v[i] = 0;

                if (flag_water == 2) Aclo_pct[i + 1] = Aclo_pct_w[j];
                else  Aclo_pct[i + 1] = Aclo_pct_a[j];

                hr[i + 1] = hr[i];
                hc[i + 1] = hc[i];
                if (Aclo_pct[i + 1] == 1)
                {
                    hr[i] = 0;
                    hc[i] = 0;
                }

                a[i + 1] = Aclo_pct[i + 1] * a[i];
                text_man[0][i + 1] = text_man[0][i];
                text_man[1][i + 1] = "clothes";

                //start parameters
                ta_air[i + 1] = ta_air[i];
                ta_air[i] = 0;
                rh_air[i + 1] = rh_air[i];
                rh_air[i] = 0;
                v_air[i + 1] = v_air[i];
                v_air[i] = 0;
                hr_start[i + 1] = hr_start[i];
                hr_start[i] = 0;
                hc_start[i + 1] = hc_start[i];
                hc_start[i] = 0;
            }

            if (flag_water == 2)
            {
                ma[i + 1] = density_w[j] * a[i + 1] * thickness_w[j] / 1000;                 // mass of clothes, kg
                Ref[i + 1] = Ref_w[j] / KOEFF;
                Rf[i + 1] = Rf_w[j] / KOEFF;
                ip[i + 1] = ip_w[j];
                thickness[i + 1] = thickness_w[j];
            }
            else
            {
                ma[i + 1] = density_a[j] * a[i + 1] * thickness_a[j] / 1000;                 // mass of clothes, kg
                Ref[i + 1] = Ref_a[j] / KOEFF;
                Rf[i + 1] = Rf_a[j] / KOEFF;
                ip[i + 1] = ip_a[j];
                thickness[i + 1] = thickness_a[j];
                Aclo_pct[i + 1] = Aclo_pct_a[j];
            }

            c[i + 1] = 0.4;                                                         // specific heat of clothes, kcal/(kg °C)
            qhk[i + 1] = a[i + 1] / (1 + FCL * Rf[i + 1]);
        }

        if (text_man[1][i + 1] == "clothes" && thickness[i + 1] <= EPS)
        {
            te[i + 1] = 0;                //net odejdi
            ta[i] = ta[i + 1];
            ta[i + 1] = 0;
            rh[i] = rh[i + 1];
            rh[i + 1] = 0;
            v[i] = v[i + 1];
            v[i + 1] = 0;
            hr[i] = hr[i + 1];
            hr[i + 1] = 0;
            hc[i] = hc[i + 1];
            hc[i + 1] = 0;
            a[i + 1] = 0;
            text_man[1][i + 1].clear();


            //start parameters
            ta_air[i] = ta_air[i + 1];
            ta_air[i + 1] = 0;
            rh_air[i] = rh_air[i + 1];
            rh_air[i + 1] = 0;
            v_air[i] = v_air[i + 1];
            v_air[i + 1] = 0;
            hr_start[i] = hr_start[i + 1];
            hr_start[i + 1] = 0;
            hc_start[i] = hc_start[i + 1];
            hc_start[i + 1] = 0;
        }
    }
}
//---------------------------------------------------------------------------

void bioman::No_Clothes(void)
{
    for (int i = 0; i < m; i++)
    {
        if (thickness[i] > EPS)
        {
            ma[i] = 0;                                                            // mass of clothes, kg
        }

        Ref[i] = 0;
        Rf[i] = 0;
        ip[i] = 0;
        thickness[i] = 0;
        Aclo_pct[i] = 0;
    }
}
//---------------------------------------------------------------------------

void bioman::fOpen_FABRIC_current_Csv(void)
{
    string Line_f0;
    bool flag_error_clo = 0;
    ifstream File_FABRIC;  //https://purecodecpp.com/archives/2751
    File_FABRIC.open(Data_FABRIC_Path.c_str());
    if (File_FABRIC.is_open() && !File_FABRIC.eof())
    {
        getline(File_FABRIC, Line_f0);
        if (Line_f0 == "Code;Fabric type/construction;Fibre content;REF, m^2·kPa/W;RF, m^2·°C/W;IP;Density, kg/m^3;Thickness, mm;Code_old")
        {
            while (!File_FABRIC.eof())
            {
                getline(File_FABRIC, Line_f0);
                fReading_Cloth(Line_f0);
            }
        }
        else flag_error_clo = 1;
        File_FABRIC.close();
    }
    else flag_error_clo = 1;
    if (flag_error_clo)
    {
        flag_error_b = 1;
        Text_Log_Error_b += "Error opening file \"FABRIC_current.csv\" ";
    }
}
//---------------------------------------------------------------------------

void bioman::fReading_Cloth(string Line_clo)
{
    int fabric_1;
    double Ref1;
    double Rf1;
    double ip1;
    double density1;
    double thickness1;

    vector <string> Parameters_clo;
    size_t prev_1;
    size_t next_1;

    prev_1 = 0;
    while ((next_1 = Line_clo.find(";", prev_1)) != string::npos)
    {
        Parameters_clo.push_back(Line_clo.substr(prev_1, next_1 - prev_1));
        prev_1 = next_1 + 1;
    }

    fabric_1 = atoi(Parameters_clo[0].c_str());
    Ref1 = fString_to_Double(Parameters_clo[3]);
    Rf1 = fString_to_Double(Parameters_clo[4]);
    ip1 = fString_to_Double(Parameters_clo[5]);
    density1 = fString_to_Double(Parameters_clo[6]);
    thickness1 = fString_to_Double(Parameters_clo[7]);;

    if (flag_immersion) for (int i = 0; i < M_Count_Cylinder_14; i++)
    {
        if (Cloth_code[i] == fabric_1)
        {
            Ref_w[i] = Ref1;
            Rf_w[i] = Rf1;
            ip_w[i] = ip1;
            density_w[i] = density1;
            thickness_w[i] = thickness1;
            Aclo_pct_w[i] = 1;
            if (i == 0)Aclo_pct_w[i] = 0.5;                                            // head is covered with clothes by 50%
        }
    }
    else for (int i = 0; i < M_Count_Cylinder_14; i++)
    {
        if (Cloth_code[i] == fabric_1)
        {
            Ref_a[i] = Ref1;
            Rf_a[i] = Rf1;
            ip_a[i] = ip1;
            density_a[i] = density1;
            thickness_a[i] = thickness1;
            Aclo_pct_a[i] = 1;
            if (i == 0)Aclo_pct_a[i] = 0.5;                                            // head is covered with clothes by 50%
        }
    }
}
//---------------------------------------------------------------------------

void bioman::Calc1(void)
{
    int j;
    double mii;

    tb += t_h * (flv_mean - sum_flv_man * tb) / (bv * ro * ce);
    /*
    double tb_control;
    tb_control=flv_mean/sum_flv_man;
    if(ework>750)
    {
       if((tb>(tb_control+0.1)) || (tb<(tb_control-0.1)))
          tb = tb_control;
    }
    */

    if (tb > Tb_start || ework > 0) h_r = 20 * tb - 661 + 0.1 * ework / KOEFF;                //issledovat' umen'schenie chastoty


    //Body temperature, muscle temperature and heat skin flows
    mii = 0; mmust = 0; mft = 0;
    j = 0;
    for (int i = i_torso; i < m - 1; i++)
    {                                                                           // <=EPS equivalently ==0 // >=EPS equivalently !=0
        if (a[i] >= EPS && mr[i] >= EPS)
        {
            if (mr[i - 2] >= EPS)
            {
                mii += te[i - 2] * ma[i - 2];
                mft += te[i - 1] * ma[i - 1];
                if (Qe > sum_mr_man + 10) mmust += te[i - 2] * k_ext[j];
            }
            else
            {
                mii += te[i - 1] * ma[i - 1];
                if (Qe > sum_mr_man + 10) mmust += te[i - 1] * k_ext[j];
            }
            j++;
        }
    }
    mft /= sum_ma_fat;                                                            // Mean fat temperature
    if (Qe <= sum_mr_man + 10)
    {
        mmust = mii / sum_ma_muscle;                                                 // Mean muscle temperature in rest
        if (m != m_clothes) mi = 0.7 * te[i_torso] + 0.3 * Tsk;
        else mi = 0.7 * te[i_torso + 1] + 0.3 * Tsk;
    }
    else
    {
        if (m != m_clothes) mi = 0.7 * te[i_torso] + 0.2 * mmust + 0.1 * Tsk;
        else mi = 0.7 * te[i_torso + 1] + 0.2 * mmust + 0.1 * Tsk;
    }
    sq2 = sq = sq3 = sq4 = sq4_w = sm = 0;
    MR_total = 0;
    for (int i = 0; i < m; i++)
    {
        MR_total += mr[i] + Msh_loc[i] + Mwork_loc[i];
        if (a[i] >= EPS)                                                           // <=EPS equivalently ==0 // >=EPS equivalently !=0
        {
            sq2 += ro * ce * fla[i] * (tb - te[i]);
            if (thickness[i + 1] >= EPS)
            {
                sq3 += (1 - Aclo_pct[i + 1]) * a[i] * hr[i] * (te[i] - ta[i + 1]);                 // partially clothed skin
            }
            else
            {
                sq3 += a[i] * hr[i] * (te[i] - ta[i]);                                     // naked (nude) skin  or clothing
            }

            if (flag_water == 2 && (locus_immersion[i] || locus_immersion[i - 1]))
            {
                if (thickness[i] >= EPS)                                              // for clothes
                {
                    sq4_w += qh_clo[i] * (te[i] - ta[i]);                               // конвективно-кондуктивная передача тепла от одежды в воду
                    sq += qh_clo[i] * (te[i] - ta[i]);
                }
                else
                {
                    if (thickness[i + 1] >= EPS)                                         // partially clothed skin
                    {
                        sq4_w += (1 - Aclo_pct[i + 1]) * a[i] * hc[i] * (te[i] - ta[i + 1]);
                        sq += (1 - Aclo_pct[i + 1]) * a[i] * (hc[i] + hr[i]) * (te[i] - ta[i + 1]);
                    }
                    else                                                            // naked (nude) skin
                    {
                        sq4_w += a[i] * hc[i] * (te[i] - ta[i]);                           // конвективно-кондуктивная передача тепла от кожи в воду
                        sq += a[i] * (hc[i] + hr[i]) * (te[i] - ta[i]);                      //heat transfer
                    }
                }
            }
            else
            {
                if (thickness[i] >= EPS)                                              // for clothes
                {
                    sq4 += qh_clo[i] * (te[i] - ta[i]);                                 // heat transfer from clothes in air
                    sq += qh_clo[i] * (te[i] - ta[i]);
                }
                else                                                               // for skin
                {
                    if (thickness[i + 1] >= EPS)                                         // partially clothed skin
                    {
                        sq4 += (1 - Aclo_pct[i + 1]) * a[i] * hc[i] * sqrt(10 * v[i + 1]) * (te[i] - ta[i + 1]);
                        sq += (1 - Aclo_pct[i + 1]) * a[i] * ((sqrt(10 * v[i + 1]) * hc[i]) + hr[i]) * (te[i] - ta[i + 1]);
                    }
                    else                                                            // naked (nude) skin
                    {
                        sq4 += a[i] * hc[i] * sqrt(10 * v[i]) * (te[i] - ta[i]);               // convection
                        sq += a[i] * ((sqrt(10 * v[i]) * hc[i]) + hr[i]) * (te[i] - ta[i]);      // heat transfer
                    }
                }
            }
            sm += qhk[i] * (te[i - 1] - te[i]);
        }
    }
}
//---------------------------------------------------------------------------

//**********begin graph**************
void bioman::BuilderGraphBP(void)
{
    Time_X[0] = floor(t * 1000 + 0.5) / 1000.0;
    Blood_T[0] = floor(tb * 1000) / 1000;
    Heart_Rate[0] = floor(h_r + 0.5);

    MSkinT[0] = floor(Tsk_start * 1000) / 1000;
    MBodyT[0] = floor(mi * 1000) / 1000;
    MMusclesT[0] = floor(mmust * 1000) / 1000;
    MFatT[0] = floor(mft * 1000) / 1000;
    Metabolism_skin[0] = floor(MR_sk * 1000) / 1000;
    Skin_heat_flow[0] = floor(sq2 * 1000) / 1000;
    Skin_conduction[0] = floor(smn * 1000) / 1000;                       //?
    M_Radiation[0] = floor(sqn3 * 1000) / 1000;                          //?
    Air_convection[0] = floor(sqn4 * 1000) / 1000;                       //?
    Water_convection[0] = 0;
    Total_metabolism[0] = floor(sum_mr_man + 0.5);
    Respiratory_losses[0] = floor(r * 1000) / 1000;
    M_Balance[0] = floor(s3 * 1000) / 1000;
    Cardiac_output[0] = floor(sum_flv_man * 1000) / 1000;
    Stroke_volume[0] = floor(1000 * 16.7 * sum_flv_man / h_r) / 1000;

    Required_evaporation[0] = floor(e_skin_start * 1000) / 1000;               //?
    Possible_evaporation[0] = floor(ESWmax * 1000) / 1000;
    Real_evaporation[0] = e_skin_start;                                    //?
    Dripping_sw_rate[0] = 0;
    Evaporation_sw_rate[0] = floor((e_skin_start / 0.58) * 1000) / 1000;         //?
    Water_losses[0] = floor(WL + 0.5);
    M_Wettedness[0] = 0;
    Sweat_efficiancy[0] = Sw_eff;
    Skin_blood_flow[0] = floor(FLV_sk_start * 1000) / 1000;                    //?
    Muscles_blood_flow[0] = floor(FLV_m_start * 1000) / 1000;        //?
    Shivering_total[0] = 0;              //floor(MR_m_start*1000)/1000;
    Metabolic_rate[0] = floor(ework + sum_mr_man + 0.5);

    Heat_Transmission_M6[0] = floor(sqn * 1000) / 1000;                  //???

    for (int i = 0; i < m; i++)
    {
        Evap_sw_rate_loc[i][0] = floor(e[i] / 0.58 * 1000) / 1000;
        Dripping_loc[i][0] = 0;
        Wettedness_loc[i][0] = 0;
        temp[i][0] = floor(te[i] * 1000) / 1000;
        Blood_flow_loc[i][0] = floor(flv[i] * 1000) / 1000;
        Shivering_loc[i][0] = floor(Msh_loc[i] * 1000) / 1000;
        Real_evap_loc[i][0] = floor(e[i] * 1000) / 1000;
        Possible_evap_loc[i][0] = floor(ESWm[i] * 1000) / 1000;
        Activity_MET_loc[i][0] = Mwork_loc[i] + mr_start[i];
    }

    if (flag_water == 2)
    {
        t_water_gr[0] = temp_water;
        t_air_gr[0] = t_air_imm;
    }
    else
    {
        t_air_gr[0] = total_ta;
        t_water_gr[0] = 0;
    }
    Qe_gr[0] = floor(sum_mr_man + 0.5);
    RiskBodyTemp[0] = 0;
    RiskWaterLoses[0] = 0;
}
//---------------------------------------------------------------------------

void bioman::BuilderGraphAP(void)
{
    Time_X[t_step] = floor(t * 1000 + 0.5) / 1000.0;
    Blood_T[t_step] = floor(tb * 1000) / 1000;
    Heart_Rate[t_step] = floor(h_r + 0.5);

    MSkinT[t_step] = floor(Tsk * 1000) / 1000;
    MBodyT[t_step] = floor(mi * 1000) / 1000;
    MMusclesT[t_step] = floor(mmust * 1000) / 1000;
    MFatT[t_step] = floor(mft * 1000) / 1000;
    Metabolism_skin[t_step] = floor(MR_sk * 1000) / 1000;
    Skin_heat_flow[t_step] = floor(sq2 * 1000) / 1000;
    Skin_conduction[t_step] = floor(sm * 1000) / 1000;
    M_Radiation[t_step] = floor(sq3 * 1000) / 1000;
    Air_convection[t_step] = floor(sq4 * 1000) / 1000;
    Water_convection[t_step] = -floor(sq4_w + 0.5);
    Total_metabolism[t_step] = floor(MR_total + 0.5);
    Respiratory_losses[t_step] = floor(r * 1000) / 1000;
    M_Balance[t_step] = floor(s3 * 1000) / 1000;
    Cardiac_output[t_step] = floor(sum_flv_man * 1000) / 1000;
    Stroke_volume[t_step] = floor(1000 * 16.67 * sum_flv_man / h_r) / 1000;

    Required_evaporation[t_step] = floor(esw * 1000) / 1000;
    Possible_evaporation[t_step] = floor(ESWmax * 1000) / 1000;
    Real_evaporation[t_step] = ESWW;
    Dripping_sw_rate[t_step] = SWdrip;
    Evaporation_sw_rate[t_step] = floor(sw * 1000) / 1000;
    Water_losses[t_step] = floor(WL + 0.5);
    M_Wettedness[t_step] = floor(wet * 1000) / 1000;
    Sweat_efficiancy[t_step] = Sw_eff;
    Skin_blood_flow[t_step] = floor(fls * 1000) / 1000;
    Muscles_blood_flow[t_step] = floor(flm * 1000) / 1000;
    Shivering_total[t_step] = floor(Msh + 0.5);
    Metabolic_rate[t_step] = floor(ework + sum_mr_man + 0.5);

    Heat_Transmission_M6[t_step] = floor(sq * 1000) / 1000;

    for (int i = 0; i < m; i++)
    {
        Evap_sw_rate_loc[i][t_step] = floor(e[i] / 0.58 * 1000) / 1000;
        Dripping_loc[i][t_step] = floor(SWdrip_loc[i] * 1000) / 1000;
        Wettedness_loc[i][t_step] = floor(we[i] * 1000) / 1000;
        temp[i][t_step] = floor(te[i] * 1000) / 1000;
        Blood_flow_loc[i][t_step] = floor(flv[i] * 1000) / 1000;
        Shivering_loc[i][t_step] = floor(Msh_loc[i] * 1000) / 1000;
        Real_evap_loc[i][t_step] = floor(e[i] * 1000) / 1000;
        Possible_evap_loc[i][t_step] = floor(ESWm[i] * 1000) / 1000;
        Activity_MET_loc[i][t_step] = Mwork_loc[i] + mr_start[i];
    }

    if (flag_water == 2)
    {
        t_water_gr[t_step] = temp_water;
        t_air_gr[t_step] = t_air_imm;
    }
    else
    {
        t_air_gr[t_step] = total_ta;                                               //Peredelat', uchest' local!!!
        t_water_gr[t_step] = 0;
    }
    Qe_gr[t_step] = (double)Qe;
}
//**********end graph************
//---------------------------------------------------------------------------

void bioman::SaveEnv(void)                                                    //Environment
{
    for (int i = 0; i < m; i++)                                                     //raschet davleniya nasischennix parov v srede
    {
        if (fabs(ta[i]) >= EPS || rh[i] >= EPS || v[i] >= EPS)
        {
            ta[i] = total_ta;
            rh[i] = total_rh;
            v[i] = total_v;
        }
    }
    Calc_Penv();
}
//---------------------------------------------------------------------------

void bioman::Calc_Penv(void)                                                    //Environment
{
    for (int i = 0; i < m; i++)                                                     //raschet davleniya nasischennix parov v srede
    {
        if (fabs(ta[i]) >= EPS || rh[i] >= EPS || v[i] >= EPS)
        {
            Penv[i] = 0.1 * exp(18.956 - (4030.18 / (ta[i] + 235))); // kPa;
        }
        else Penv[i] = 0;
    }
}
//---------------------------------------------------------------------------

void bioman::No_Immersion(void)                                              //Immersion
{
    flag_water = 0;
    t_on_water = 0;
    t_off_water = 0;
    temp_water = 0;
    t_air_imm = 0;
    rh_air_imm = 0;
    v_air_imm = 0;

    for (int i = 0; i < M_Compartment_Max; i++)
    {
        hc_imm_fr[i] = 0;
        hc_imm_f0_ex[i] = 0;
        locus_immersion[i] = 0;
    }

    KPD_water = 0.06;
}
//---------------------------------------------------------------------------

void bioman::Input_Immersion(void)                                           //Immersion
{
    int i, j;

    t_on_water = t_ex_start;
    t_off_water = t_ex_finish;
    Qe_water = Qe_air;
    t_air_imm = total_ta;
    rh_air_imm = total_rh;
    v_air_imm = total_v;

    if (i_type == 11)
    {
        KPD_water = 0.2;     //Run
        j = 0;               //Run
    }
    else if (i_type == 12)
    {
        KPD_water = 0.06;    //Swimming
        j = 2;               //Free swimm
    }
    else
    {
        KPD_water = 0;       //Rest
        j = 0;               //Run
    }

    for (i = 0; i < M_Count_Cylinder_14; i++)
    {
        k_ext_water[i] = k_ext_all[j][i];                                          // Free swimm
    }

    if (Qe_water > 250 * KOEFF) hc_water = 580;
    else hc_water = 230;

    j = 0;
    for (i = 0; i < m; i++)
    {                                                                           // <=EPS equivalently ==0 // >=EPS equivalently !=0
        if (mr[i] >= EPS && a[i] >= EPS && j < M_Count_Cylinder_14)
        {
            locus_immersion[i] = Water_Level[j];
            j++;
        }
    }

    flag_water = 1;
}
//---------------------------------------------------------------------------

void bioman::Open_Immersion(void)                                            // Immersion
{
    int i_temp;
    double initial_hc_imm_fr[M_Count_Cylinder_14][41] = {                         // without exercise
          {1.5339607e+003, 1.5318858e+003, 1.5289086e+003, 1.5254683e+003, 1.5215721e+003, 1.5171542e+003, 1.5122311e+003, 1.5067569e+003, 1.5007366e+003, 1.4940658e+003, 1.4867739e+003, 1.4788345e+003, 1.4701219e+003, 1.4606474e+003, 1.4503826e+003, 1.4392057e+003, 1.4270893e+003, 1.4139486e+003, 1.3996691e+003, 1.3841876e+003, 1.3673637e+003, 1.3490191e+003, 1.3290347e+003, 1.3071223e+003, 1.2830498e+003, 1.2564750e+003, 1.2269049e+003, 1.1938009e+003, 1.1562532e+003, 1.1131133e+003, 1.0624461e+003, 1.0011410e+003, 9.2314738e+002, 8.1412055e+002, 6.1564162e+002, 6.3304197e+002, 8.2950273e+002, 9.4422325e+002, 1.0298514e+003, 1.0998663e+003, 1.1599468e+003},
          {1.1376111e+003, 1.1361417e+003, 1.1340250e+003, 1.1315702e+003, 1.1287831e+003, 1.1256153e+003, 1.1220797e+003, 1.1181428e+003, 1.1138090e+003, 1.1090018e+003, 1.1037437e+003, 1.0980164e+003, 1.0917279e+003, 1.0848879e+003, 1.0774770e+003, 1.0694071e+003, 1.0606602e+003, 1.0511763e+003, 1.0408739e+003, 1.0297107e+003, 1.0175881e+003, 1.0043807e+003, 9.9000919e+002, 9.7427152e+002, 9.5701149e+002, 9.3799728e+002, 9.1689422e+002, 8.9334919e+002, 8.6675849e+002, 8.3638834e+002, 8.0101271e+002, 7.5874293e+002, 7.0607746e+002, 6.3549019e+002, 5.2337477e+002, 3.5341021e+002, 5.7434114e+002, 6.7254774e+002, 7.4188778e+002, 7.9717236e+002, 8.4393119e+002},
          {1.5405876e+003, 1.5384765e+003, 1.5354506e+003, 1.5319575e+003, 1.5280042e+003, 1.5235246e+003, 1.5185348e+003, 1.5129886e+003, 1.5068908e+003, 1.5001362e+003, 1.4927537e+003, 1.4847167e+003, 1.4758983e+003, 1.4663093e+003, 1.4559204e+003, 1.4446086e+003, 1.4323454e+003, 1.4190444e+003, 1.4045892e+003, 1.3889144e+003, 1.3718769e+003, 1.3532948e+003, 1.3330447e+003, 1.3108323e+003, 1.2864179e+003, 1.2594487e+003, 1.2294166e+003, 1.1957606e+003, 1.1575367e+003, 1.1135396e+003, 1.0617324e+003, 9.9879801e+002, 9.1818255e+002, 8.0382578e+002, 5.8280860e+002, 6.6497208e+002, 8.4722031e+002, 9.5838410e+002, 1.0424241e+003, 1.1115711e+003, 1.1711276e+003},
          {1.5405876e+003, 1.5384765e+003, 1.5354506e+003, 1.5319575e+003, 1.5280042e+003, 1.5235246e+003, 1.5185348e+003, 1.5129886e+003, 1.5068908e+003, 1.5001362e+003, 1.4927537e+003, 1.4847167e+003, 1.4758983e+003, 1.4663093e+003, 1.4559204e+003, 1.4446086e+003, 1.4323454e+003, 1.4190444e+003, 1.4045892e+003, 1.3889144e+003, 1.3718769e+003, 1.3532948e+003, 1.3330447e+003, 1.3108323e+003, 1.2864179e+003, 1.2594487e+003, 1.2294166e+003, 1.1957606e+003, 1.1575367e+003, 1.1135396e+003, 1.0617324e+003, 9.9879801e+002, 9.1818255e+002, 8.0382578e+002, 5.8280860e+002, 6.6497208e+002, 8.4722031e+002, 9.5838410e+002, 1.0424241e+003, 1.1115711e+003, 1.1711276e+003},
          {1.5412636e+003, 1.5391677e+003, 1.5361617e+003, 1.5326897e+003, 1.5287585e+003, 1.5243022e+003, 1.5193372e+003, 1.5138173e+003, 1.5077474e+003, 1.5010225e+003, 1.4936718e+003, 1.4856689e+003, 1.4768871e+003, 1.4673376e+003, 1.4569915e+003, 1.4457262e+003, 1.4335137e+003, 1.4202684e+003, 1.4058745e+003, 1.3902680e+003, 1.3733068e+003, 1.3548105e+003, 1.3346582e+003, 1.3125582e+003, 1.2882745e+003, 1.2614599e+003, 1.2316135e+003, 1.1981863e+003, 1.1602520e+003, 1.1166356e+003, 1.0653558e+003, 1.0032101e+003, 9.2393079e+002, 8.1246132e+002, 6.0487940e+002, 6.4841390e+002, 8.3928928e+002, 9.5285938e+002, 1.0380828e+003, 1.1079476e+003, 1.1679913e+003},
          {1.5412636e+003, 1.5391677e+003, 1.5361617e+003, 1.5326897e+003, 1.5287585e+003, 1.5243022e+003, 1.5193372e+003, 1.5138173e+003, 1.5077474e+003, 1.5010225e+003, 1.4936718e+003, 1.4856689e+003, 1.4768871e+003, 1.4673376e+003, 1.4569915e+003, 1.4457262e+003, 1.4335137e+003, 1.4202684e+003, 1.4058745e+003, 1.3902680e+003, 1.3733068e+003, 1.3548105e+003, 1.3346582e+003, 1.3125582e+003, 1.2882745e+003, 1.2614599e+003, 1.2316135e+003, 1.1981863e+003, 1.1602520e+003, 1.1166356e+003, 1.0653558e+003, 1.0032101e+003, 9.2393079e+002, 8.1246132e+002, 6.0487940e+002, 6.4841390e+002, 8.3928928e+002, 9.5285938e+002, 1.0380828e+003, 1.1079476e+003, 1.1679913e+003},
          {1.5890627e+003, 1.5870388e+003, 1.5841196e+003, 1.5807304e+003, 1.5768793e+003, 1.5724991e+003, 1.5676078e+003, 1.5621590e+003, 1.5561593e+003, 1.5495017e+003, 1.5422185e+003, 1.5342843e+003, 1.5255710e+003, 1.5160929e+003, 1.5058237e+003, 1.4946408e+003, 1.4825203e+003, 1.4693794e+003, 1.4551058e+003, 1.4396422e+003, 1.4228527e+003, 1.4045651e+003, 1.3846722e+003, 1.3628963e+003, 1.3390254e+003, 1.3127441e+003, 1.2835967e+003, 1.2511077e+003, 1.2144603e+003, 1.1726731e+003, 1.1241089e+003, 1.0662762e+003, 9.9461290e+002, 8.9955652e+002, 7.5276339e+002, 3.8676147e+002, 7.8287851e+002, 9.2706763e+002, 1.0267581e+003, 1.1055478e+003, 1.1718654e+003},
          {1.5890627e+003, 1.5870388e+003, 1.5841196e+003, 1.5807304e+003, 1.5768793e+003, 1.5724991e+003, 1.5676078e+003, 1.5621590e+003, 1.5561593e+003, 1.5495017e+003, 1.5422185e+003, 1.5342843e+003, 1.5255710e+003, 1.5160929e+003, 1.5058237e+003, 1.4946408e+003, 1.4825203e+003, 1.4693794e+003, 1.4551058e+003, 1.4396422e+003, 1.4228527e+003, 1.4045651e+003, 1.3846722e+003, 1.3628963e+003, 1.3390254e+003, 1.3127441e+003, 1.2835967e+003, 1.2511077e+003, 1.2144603e+003, 1.1726731e+003, 1.1241089e+003, 1.0662762e+003, 9.9461290e+002, 8.9955652e+002, 7.5276339e+002, 3.8676147e+002, 7.8287851e+002, 9.2706763e+002, 1.0267581e+003, 1.1055478e+003, 1.1718654e+003},
          {1.4653400e+003, 1.4633515e+003, 1.4604989e+003, 1.4572035e+003, 1.4534719e+003, 1.4492416e+003, 1.4445279e+003, 1.4392870e+003, 1.4335238e+003, 1.4271384e+003, 1.4201586e+003, 1.4125593e+003, 1.4042202e+003, 1.3951521e+003, 1.3853275e+003, 1.3746300e+003, 1.3630331e+003, 1.3504557e+003, 1.3367879e+003, 1.3219689e+003, 1.3058641e+003, 1.2883026e+003, 1.2691696e+003, 1.2481887e+003, 1.2251366e+003, 1.1996844e+003, 1.1713579e+003, 1.1396380e+003, 1.1036485e+003, 1.0622802e+003, 1.0136627e+003, 9.5477997e+002, 8.7974268e+002, 7.7447556e+002, 5.8019352e+002, 6.1203383e+002, 7.9582470e+002, 9.0441296e+002, 9.8573280e+002, 1.0523316e+003, 1.1095350e+003},
          {1.4653400e+003, 1.4633515e+003, 1.4604989e+003, 1.4572035e+003, 1.4534719e+003, 1.4492416e+003, 1.4445279e+003, 1.4392870e+003, 1.4335238e+003, 1.4271384e+003, 1.4201586e+003, 1.4125593e+003, 1.4042202e+003, 1.3951521e+003, 1.3853275e+003, 1.3746300e+003, 1.3630331e+003, 1.3504557e+003, 1.3367879e+003, 1.3219689e+003, 1.3058641e+003, 1.2883026e+003, 1.2691696e+003, 1.2481887e+003, 1.2251366e+003, 1.1996844e+003, 1.1713579e+003, 1.1396380e+003, 1.1036485e+003, 1.0622802e+003, 1.0136627e+003, 9.5477997e+002, 8.7974268e+002, 7.7447556e+002, 5.8019352e+002, 6.1203383e+002, 7.9582470e+002, 9.0441296e+002, 9.8573280e+002, 1.0523316e+003, 1.1095350e+003},
          {1.4657187e+003, 1.4637386e+003, 1.4608972e+003, 1.4576136e+003, 1.4538944e+003, 1.4496771e+003, 1.4449772e+003, 1.4397511e+003, 1.4340035e+003, 1.4276346e+003, 1.4206726e+003, 1.4130923e+003, 1.4047737e+003, 1.3957276e+003, 1.3859269e+003, 1.3752553e+003, 1.3636868e+003, 1.3511404e+003, 1.3375068e+003, 1.3227258e+003, 1.3066636e+003, 1.2891498e+003, 1.2700712e+003, 1.2491528e+003, 1.2261733e+003, 1.2008068e+003, 1.1725833e+003, 1.1409900e+003, 1.1051604e+003, 1.0640017e+003, 1.0156735e+003, 9.5722077e+002, 8.8290457e+002, 7.7916112e+002, 5.9128004e+002, 6.0192805e+002, 7.9123826e+002, 9.0125576e+002, 9.8326478e+002, 1.0502777e+003, 1.1077605e+003},
          {1.4657187e+003, 1.4637386e+003, 1.4608972e+003, 1.4576136e+003, 1.4538944e+003, 1.4496771e+003, 1.4449772e+003, 1.4397511e+003, 1.4340035e+003, 1.4276346e+003, 1.4206726e+003, 1.4130923e+003, 1.4047737e+003, 1.3957276e+003, 1.3859269e+003, 1.3752553e+003, 1.3636868e+003, 1.3511404e+003, 1.3375068e+003, 1.3227258e+003, 1.3066636e+003, 1.2891498e+003, 1.2700712e+003, 1.2491528e+003, 1.2261733e+003, 1.2008068e+003, 1.1725833e+003, 1.1409900e+003, 1.1051604e+003, 1.0640017e+003, 1.0156735e+003, 9.5722077e+002, 8.8290457e+002, 7.7916112e+002, 5.9128004e+002, 6.0192805e+002, 7.9123826e+002, 9.0125576e+002, 9.8326478e+002, 1.0502777e+003, 1.1077605e+003},
          {1.6046047e+003, 1.6024891e+003, 1.5994468e+003, 1.5959244e+003, 1.5919296e+003, 1.5873941e+003, 1.5823355e+003, 1.5767063e+003, 1.5705124e+003, 1.5636450e+003, 1.5561357e+003, 1.5479578e+003, 1.5389807e+003, 1.5292174e+003, 1.5186395e+003, 1.5071214e+003, 1.4946361e+003, 1.4810974e+003, 1.4663883e+003, 1.4504461e+003, 1.4331285e+003, 1.4142544e+003, 1.3937067e+003, 1.3711930e+003, 1.3464835e+003, 1.3192379e+003, 1.2889657e+003, 1.2551409e+003, 1.2168698e+003, 1.1730476e+003, 1.1218226e+003, 1.0602893e+003, 9.8295213e+002, 8.7751145e+002, 7.0137736e+002, 5.8756406e+002, 8.3692411e+002, 9.6641102e+002, 1.0603395e+003, 1.1361228e+003, 1.2006506e+003},
          {1.6046047e+003, 1.6024891e+003, 1.5994468e+003, 1.5959244e+003, 1.5919296e+003, 1.5873941e+003, 1.5823355e+003, 1.5767063e+003, 1.5705124e+003, 1.5636450e+003, 1.5561357e+003, 1.5479578e+003, 1.5389807e+003, 1.5292174e+003, 1.5186395e+003, 1.5071214e+003, 1.4946361e+003, 1.4810974e+003, 1.4663883e+003, 1.4504461e+003, 1.4331285e+003, 1.4142544e+003, 1.3937067e+003, 1.3711930e+003, 1.3464835e+003, 1.3192379e+003, 1.2889657e+003, 1.2551409e+003, 1.2168698e+003, 1.1730476e+003, 1.1218226e+003, 1.0602893e+003, 9.8295213e+002, 8.7751145e+002, 7.0137736e+002, 5.8756406e+002, 8.3692411e+002, 9.6641102e+002, 1.0603395e+003, 1.1361228e+003, 1.2006506e+003} };

    double initial_hc_imm_f0_ex[M_Count_Cylinder_14][41] = {                      // with exercise
          {1.5136032e+002, 1.4995903e+002, 1.4825164e+002, 1.4657837e+002, 1.4494458e+002, 1.4335214e+002, 1.4179632e+002, 1.4027212e+002, 1.3878772e+002, 1.3733825e+002, 1.3592157e+002, 1.3453546e+002, 1.3318186e+002, 1.3186169e+002, 1.3056874e+002, 1.2930504e+002, 1.2807099e+002, 1.2686092e+002, 1.2568290e+002, 1.2452992e+002, 1.2339893e+002, 1.2229306e+002, 1.2121542e+002, 1.2015753e+002, 1.1912353e+002, 1.1811337e+002, 1.1712135e+002, 1.1615424e+002, 1.1520420e+002, 1.1427819e+002, 1.1336846e+002, 1.1247877e+002, 1.1160685e+002, 1.1075427e+002, 1.0991639e+002, 1.0909941e+002, 1.0829528e+002, 1.0750924e+002, 1.0673845e+002, 1.0598465e+002, 1.0524218e+002},
          {4.5317462e+001, 4.4897914e+001, 4.4386718e+001, 4.3885739e+001, 4.3396582e+001, 4.2919803e+001, 4.2453988e+001, 4.1997639e+001, 4.1553209e+001, 4.1119238e+001, 4.0695082e+001, 4.0280079e+001, 3.9874808e+001, 3.9479549e+001, 3.9092437e+001, 3.8714083e+001, 3.8344608e+001, 3.7982312e+001, 3.7629609e+001, 3.7284408e+001, 3.6945786e+001, 3.6614689e+001, 3.6292043e+001, 3.5975309e+001, 3.5665728e+001, 3.5363285e+001, 3.5066271e+001, 3.4776718e+001, 3.4492274e+001, 3.4215026e+001, 3.3942651e+001, 3.3676279e+001, 3.3415223e+001, 3.3159962e+001, 3.2909098e+001, 3.2664495e+001, 3.2423738e+001, 3.2188396e+001, 3.1957621e+001, 3.1731931e+001, 3.1509634e+001},
          {1.5444931e+002, 1.5301942e+002, 1.5127718e+002, 1.4956976e+002, 1.4790264e+002, 1.4627770e+002, 1.4469012e+002, 1.4313481e+002, 1.4162012e+002, 1.4014108e+002, 1.3869548e+002, 1.3728109e+002, 1.3589986e+002, 1.3455275e+002, 1.3323341e+002, 1.3194391e+002, 1.3068468e+002, 1.2944992e+002, 1.2824785e+002, 1.2707135e+002, 1.2591727e+002, 1.2478884e+002, 1.2368921e+002, 1.2260973e+002, 1.2155462e+002, 1.2052385e+002, 1.1951158e+002, 1.1852473e+002, 1.1755530e+002, 1.1661040e+002, 1.1568210e+002, 1.1477426e+002, 1.1388454e+002, 1.1301456e+002, 1.1215958e+002, 1.1132593e+002, 1.1050539e+002, 1.0970331e+002, 1.0891679e+002, 1.0814760e+002, 1.0738998e+002},
          {1.5444931e+002, 1.5301942e+002, 1.5127718e+002, 1.4956976e+002, 1.4790264e+002, 1.4627770e+002, 1.4469012e+002, 1.4313481e+002, 1.4162012e+002, 1.4014108e+002, 1.3869548e+002, 1.3728109e+002, 1.3589986e+002, 1.3455275e+002, 1.3323341e+002, 1.3194391e+002, 1.3068468e+002, 1.2944992e+002, 1.2824785e+002, 1.2707135e+002, 1.2591727e+002, 1.2478884e+002, 1.2368921e+002, 1.2260973e+002, 1.2155462e+002, 1.2052385e+002, 1.1951158e+002, 1.1852473e+002, 1.1755530e+002, 1.1661040e+002, 1.1568210e+002, 1.1477426e+002, 1.1388454e+002, 1.1301456e+002, 1.1215958e+002, 1.1132593e+002, 1.1050539e+002, 1.0970331e+002, 1.0891679e+002, 1.0814760e+002, 1.0738998e+002},
          {1.5444931e+002, 1.5301942e+002, 1.5127718e+002, 1.4956976e+002, 1.4790264e+002, 1.4627770e+002, 1.4469012e+002, 1.4313481e+002, 1.4162012e+002, 1.4014108e+002, 1.3869548e+002, 1.3728109e+002, 1.3589986e+002, 1.3455275e+002, 1.3323341e+002, 1.3194391e+002, 1.3068468e+002, 1.2944992e+002, 1.2824785e+002, 1.2707135e+002, 1.2591727e+002, 1.2478884e+002, 1.2368921e+002, 1.2260973e+002, 1.2155462e+002, 1.2052385e+002, 1.1951158e+002, 1.1852473e+002, 1.1755530e+002, 1.1661040e+002, 1.1568210e+002, 1.1477426e+002, 1.1388454e+002, 1.1301456e+002, 1.1215958e+002, 1.1132593e+002, 1.1050539e+002, 1.0970331e+002, 1.0891679e+002, 1.0814760e+002, 1.0738998e+002},
          {1.5444931e+002, 1.5301942e+002, 1.5127718e+002, 1.4956976e+002, 1.4790264e+002, 1.4627770e+002, 1.4469012e+002, 1.4313481e+002, 1.4162012e+002, 1.4014108e+002, 1.3869548e+002, 1.3728109e+002, 1.3589986e+002, 1.3455275e+002, 1.3323341e+002, 1.3194391e+002, 1.3068468e+002, 1.2944992e+002, 1.2824785e+002, 1.2707135e+002, 1.2591727e+002, 1.2478884e+002, 1.2368921e+002, 1.2260973e+002, 1.2155462e+002, 1.2052385e+002, 1.1951158e+002, 1.1852473e+002, 1.1755530e+002, 1.1661040e+002, 1.1568210e+002, 1.1477426e+002, 1.1388454e+002, 1.1301456e+002, 1.1215958e+002, 1.1132593e+002, 1.1050539e+002, 1.0970331e+002, 1.0891679e+002, 1.0814760e+002, 1.0738998e+002},
          {1.7200037e+002, 1.7040799e+002, 1.6846777e+002, 1.6656633e+002, 1.6470975e+002, 1.6290016e+002, 1.6113218e+002, 1.5940013e+002, 1.5771332e+002, 1.5606620e+002, 1.5445633e+002, 1.5288121e+002, 1.5134302e+002, 1.4984284e+002, 1.4837357e+002, 1.4693754e+002, 1.4553522e+002, 1.4416014e+002, 1.4282147e+002, 1.4151128e+002, 1.4022605e+002, 1.3896939e+002, 1.3774480e+002, 1.3654265e+002, 1.3536765e+002, 1.3421974e+002, 1.3309244e+002, 1.3199345e+002, 1.3091386e+002, 1.2986158e+002, 1.2882779e+002, 1.2781678e+002, 1.2682596e+002, 1.2585713e+002, 1.2490498e+002, 1.2397660e+002, 1.2306282e+002, 1.2216959e+002, 1.2129370e+002, 1.2043710e+002, 1.1959338e+002},
          {1.7200037e+002, 1.7040799e+002, 1.6846777e+002, 1.6656633e+002, 1.6470975e+002, 1.6290016e+002, 1.6113218e+002, 1.5940013e+002, 1.5771332e+002, 1.5606620e+002, 1.5445633e+002, 1.5288121e+002, 1.5134302e+002, 1.4984284e+002, 1.4837357e+002, 1.4693754e+002, 1.4553522e+002, 1.4416014e+002, 1.4282147e+002, 1.4151128e+002, 1.4022605e+002, 1.3896939e+002, 1.3774480e+002, 1.3654265e+002, 1.3536765e+002, 1.3421974e+002, 1.3309244e+002, 1.3199345e+002, 1.3091386e+002, 1.2986158e+002, 1.2882779e+002, 1.2781678e+002, 1.2682596e+002, 1.2585713e+002, 1.2490498e+002, 1.2397660e+002, 1.2306282e+002, 1.2216959e+002, 1.2129370e+002, 1.2043710e+002, 1.1959338e+002},
          {1.2613360e+002, 1.2496586e+002, 1.2354303e+002, 1.2214864e+002, 1.2078715e+002, 1.1946012e+002, 1.1816360e+002, 1.1689343e+002, 1.1565643e+002, 1.1444855e+002, 1.1326798e+002, 1.1211289e+002, 1.1098488e+002, 1.0988475e+002, 1.0880728e+002, 1.0775420e+002, 1.0672583e+002, 1.0571743e+002, 1.0473575e+002, 1.0377494e+002, 1.0283244e+002, 1.0191088e+002, 1.0101285e+002, 1.0013128e+002, 9.9269610e+001, 9.8427809e+001, 9.7601122e+001, 9.6795198e+001, 9.6003497e+001, 9.5231824e+001, 9.4473713e+001, 9.3732309e+001, 9.3005705e+001, 9.2295227e+001, 9.1596989e+001, 9.0916177e+001, 9.0246069e+001, 8.9591035e+001, 8.8948712e+001, 8.8320542e+001, 8.7701815e+001},
          {1.2613360e+002, 1.2496586e+002, 1.2354303e+002, 1.2214864e+002, 1.2078715e+002, 1.1946012e+002, 1.1816360e+002, 1.1689343e+002, 1.1565643e+002, 1.1444855e+002, 1.1326798e+002, 1.1211289e+002, 1.1098488e+002, 1.0988475e+002, 1.0880728e+002, 1.0775420e+002, 1.0672583e+002, 1.0571743e+002, 1.0473575e+002, 1.0377494e+002, 1.0283244e+002, 1.0191088e+002, 1.0101285e+002, 1.0013128e+002, 9.9269610e+001, 9.8427809e+001, 9.7601122e+001, 9.6795198e+001, 9.6003497e+001, 9.5231824e+001, 9.4473713e+001, 9.3732309e+001, 9.3005705e+001, 9.2295227e+001, 9.1596989e+001, 9.0916177e+001, 9.0246069e+001, 8.9591035e+001, 8.8948712e+001, 8.8320542e+001, 8.7701815e+001},
          {1.2613360e+002, 1.2496586e+002, 1.2354303e+002, 1.2214864e+002, 1.2078715e+002, 1.1946012e+002, 1.1816360e+002, 1.1689343e+002, 1.1565643e+002, 1.1444855e+002, 1.1326798e+002, 1.1211289e+002, 1.1098488e+002, 1.0988475e+002, 1.0880728e+002, 1.0775420e+002, 1.0672583e+002, 1.0571743e+002, 1.0473575e+002, 1.0377494e+002, 1.0283244e+002, 1.0191088e+002, 1.0101285e+002, 1.0013128e+002, 9.9269610e+001, 9.8427809e+001, 9.7601122e+001, 9.6795198e+001, 9.6003497e+001, 9.5231824e+001, 9.4473713e+001, 9.3732309e+001, 9.3005705e+001, 9.2295227e+001, 9.1596989e+001, 9.0916177e+001, 9.0246069e+001, 8.9591035e+001, 8.8948712e+001, 8.8320542e+001, 8.7701815e+001},
          {1.2613360e+002, 1.2496586e+002, 1.2354303e+002, 1.2214864e+002, 1.2078715e+002, 1.1946012e+002, 1.1816360e+002, 1.1689343e+002, 1.1565643e+002, 1.1444855e+002, 1.1326798e+002, 1.1211289e+002, 1.1098488e+002, 1.0988475e+002, 1.0880728e+002, 1.0775420e+002, 1.0672583e+002, 1.0571743e+002, 1.0473575e+002, 1.0377494e+002, 1.0283244e+002, 1.0191088e+002, 1.0101285e+002, 1.0013128e+002, 9.9269610e+001, 9.8427809e+001, 9.7601122e+001, 9.6795198e+001, 9.6003497e+001, 9.5231824e+001, 9.4473713e+001, 9.3732309e+001, 9.3005705e+001, 9.2295227e+001, 9.1596989e+001, 9.0916177e+001, 9.0246069e+001, 8.9591035e+001, 8.8948712e+001, 8.8320542e+001, 8.7701815e+001},
          {1.8019086e+002, 1.7852266e+002, 1.7649004e+002, 1.7449806e+002, 1.7255308e+002, 1.7065731e+002, 1.6880514e+002, 1.6699061e+002, 1.6522347e+002, 1.6349792e+002, 1.6181140e+002, 1.6016127e+002, 1.5854983e+002, 1.5697821e+002, 1.5543898e+002, 1.5393457e+002, 1.5246547e+002, 1.5102491e+002, 1.4962249e+002, 1.4824991e+002, 1.4690348e+002, 1.4558698e+002, 1.4430408e+002, 1.4304468e+002, 1.4181373e+002, 1.4061116e+002, 1.3943017e+002, 1.3827885e+002, 1.3714785e+002, 1.3604546e+002, 1.3496245e+002, 1.3390330e+002, 1.3286529e+002, 1.3185032e+002, 1.3085284e+002, 1.2988025e+002, 1.2892296e+002, 1.2798719e+002, 1.2706959e+002, 1.2617220e+002, 1.2528831e+002},
          {1.8019086e+002, 1.7852266e+002, 1.7649004e+002, 1.7449806e+002, 1.7255308e+002, 1.7065731e+002, 1.6880514e+002, 1.6699061e+002, 1.6522347e+002, 1.6349792e+002, 1.6181140e+002, 1.6016127e+002, 1.5854983e+002, 1.5697821e+002, 1.5543898e+002, 1.5393457e+002, 1.5246547e+002, 1.5102491e+002, 1.4962249e+002, 1.4824991e+002, 1.4690348e+002, 1.4558698e+002, 1.4430408e+002, 1.4304468e+002, 1.4181373e+002, 1.4061116e+002, 1.3943017e+002, 1.3827885e+002, 1.3714785e+002, 1.3604546e+002, 1.3496245e+002, 1.3390330e+002, 1.3286529e+002, 1.3185032e+002, 1.3085284e+002, 1.2988025e+002, 1.2892296e+002, 1.2798719e+002, 1.2706959e+002, 1.2617220e+002, 1.2528831e+002} };

    i_temp = (int)temp_water;
    for (int i = 0, j = 0; i < m; i++)
    {
        if (hc[i] >= EPS && j < M_Count_Cylinder_14)                                  // <=EPS equivalently ==0 // >=EPS equivalently !=0
        {
            hc_imm_fr[i] = initial_hc_imm_fr[j][i_temp];
            hc_imm_f0_ex[i] = initial_hc_imm_f0_ex[j][i_temp];
            //hc_imm_f0_ex[i]=580*KOEFF;
            //hc_imm_f0_ex[i]=230*KOEFF;
            j++;
        }
    }
}
//---------------------------------------------------------------------------

void bioman::Initial_Contr_Resp(void)                                         // Form_Thermoregulation
{
    bool flag_31, flag_32, flag_33, flag_34;
    bool flag_41, flag_42, flag_43, flag_44;
    bool flag_13, flag_14;
    flag_31 = 1;                                                                  // CheckBox1
    flag_32 = 1;                                                                  // CheckBox2
    flag_33 = 0;                                                                  // CheckBox3
    flag_34 = 0;                                                                  // CheckBox4
    flag_41 = 1;                                                                  // CheckBox5
    flag_42 = 1;                                                                  // CheckBox6
    flag_43 = 0;                                                                  // CheckBox7
    flag_44 = 0;                                                                  // CheckBox8
    flag_13 = 1;                                                                  // CheckBox9
    flag_14 = 0;                                                                  // CheckBox10
    flag_Head_skin_flow = 0;                                                      // CheckBox_head_FLS
    flag_flv_a = 1;                                                               // CheckBox_coef_fls

    if (flag_31)
    {
        s31 = 500;
        h31 = 37.3;
    }
    else
    {
        s31 = 0;
        h31 = 10000;
    }
    if (flag_32)
    {
        s32 = 50;
        h32 = 36.5;
    }
    else
    {
        s32 = 0;
        h32 = 10000;
    }
    if (flag_33)
    {
        s33 = 0.01;
        h33 = 30;
    }
    else
    {
        s33 = 0;
        h33 = 10000;
    }
    if (flag_34)
    {
        s34 = 1;
        h34 = 10;
    }
    else
    {
        s34 = 0;
        h34 = 10000;
    }
    if (flag_41)
    {
        s41 = 6;
        h41 = 0.001;
    }
    else
    {
        s41 = 0;
        h41 = 10000;
    }
    if (flag_42)
    {
        s42 = 23;
        h42 = 0.001;
    }
    else
    {
        s42 = 0;
        h42 = 10000;
    }
    if (flag_43)
    {
        s43 = 0.1;
        h43 = 0.1;
    }
    else
    {
        s43 = 0;
        h43 = 10000;
    }
    if (flag_44)
    {
        s44 = 0.01;
        h44 = 10;
    }
    else
    {
        s44 = 0;
        h44 = 10000;
    }
    if (flag_13)
    {
        s13 = 0.01;
        h13 = 30;
    }
    else
    {
        s13 = 0;
        h13 = 10000;
    }
    if (flag_14)
    {
        s14 = 0.01;
        h14 = 30;
    }
    else
    {
        s14 = 0;
        h14 = 10000;
    }

    h11 = 0.0001;
    h12 = 0.0001;
    hdes = 0.005;
    hdev = 0.005;
    kt = 1;
    kw = 0.5;
    am1 = 200;
    ah1 = 10;
    am2 = 100;
    ah2 = 20;
    am3 = 50;
    ah3 = 10;
    am4 = 10;
    ah4 = 10;
    fls_max = 480;
    fls_min = 3;
}
//---------------------------------------------------------------------------

int bioman::Input_Exp_Param(void)
{
    int i, j;
    i_type = -1;
    i_sub_type = -1;
    Qe = 0; Qe_air = 0;

    //FLAG                                                                      // 0 - from_android
    if (_stricmp(Values_js[0].c_str(), "false") == 0) flag_android = 0;
    else if (_stricmp(Values_js[0].c_str(), "true") == 0) flag_android = 1;
    if (flag_android == -1) return 5;

    // Type of physical activity:
    i_type = atoi(Values_js[1].c_str());                                          // 1 - type_activities
    i_sub_type = atoi(Values_js[2].c_str());                                      // 2 - sub_type_activities
    if (i_type == -1 || i_sub_type == -1) return 3;

    // TIME
    t_ex_start = 0;
    t_phase = atof(Values_js[3].c_str()) / 60;                                      // 3 - duration, min
    t_phase = floor(t_phase * 1000 + 0.5) / 1000.0;                                   // duration, hour
    t_ex_finish = t_phase + t_h;
    if (t_phase == 0 || (t_ex_start >= t_ex_finish)) return 1;

    // SPEED (Activity velocity)                                                // 4 - distance, km
    v_activity_air = atof(Values_js[4].c_str()) / t_phase / 3.6;                      // speed, m/s

    // INTENSITY   Qe_air, kcal/h - physical activity (load, exercise)
    Qe_air = ceil(KOEFF * atof(Values_js[5].c_str()));                              // 5 - intensity, W    (1 kcal/h = 0.86 * 1 W)
    Energy_Cost_Run();
    if (Qe_air == 0) return 2;

    // ENVIRONMENT
    total_v = atof(Values_js[7].c_str());                                         // 7 - wind_velocity
    total_rh = atof(Values_js[8].c_str()) / 100;                                    // 8 - humidity
    total_ta = atof(Values_js[9].c_str());                                        // 9 - air_temperature, °C
    if (total_rh <= EPS || total_v <= EPS) return 4;

    // CLOTHES
    for (i = 0, j = 0; i < M_Count_Cylinder_8, j < M_Count_Cylinder_14; i++, j++)        // 10-17
    {
        Cloth_code[j] = atoi(Values_js[M_json_i_clo + i].c_str());
        if (j > 1)Cloth_code[j + 1] = Cloth_code[j++];
        if (Cloth_code[j] != 0) flag_cloth = 1;
    }

    // WATER
    if (flag_immersion)
    {
        temp_water = atof(Values_js[M_json_i_water].c_str());                      // 18       // water_temperature

        flag_immersion = 0;
        for (i = 0, j = 0; i < M_Count_Cylinder_8, j < M_Count_Cylinder_14; i++, j++)     // 19-26
        {
            Water_Level[j] = atoi(Values_js[M_json_i_water + 1 + i].c_str());
            if (j > 1)Water_Level[j + 1] = Water_Level[j++];
            if (Water_Level[j] != 0) flag_immersion = 1;
        }
    }

    return 0;
}
//---------------------------------------------------------------------------

void bioman::Energy_Cost_Run(void)
{
    /*double xq;
    double k_MET;
    k_MET=0.0175*sum_ma_man*60;                                                 // 1 MET = 0.0175 kcal/kg

    if(v_activity_air<0.4) Qe_air=ceil(1.3*k_MET);
    else
    {
       if(v_activity_air<4.72) xq=1;
       else if(v_activity_air>5.49) xq=0.86;
       else xq=4.6997538/v_activity_air+0.0034895;                              // Energy cost of running

       Qe_air=ceil(xq*sum_ma_man*v_activity_air*3.6);                           // Qe_air, kcal/h - physical activity (load, exercise)
    }*/

    int j;
    if (i_type == 1)
    {
        KPD_air = 0.2;
        j = 0;             //Run
    }
    else if (i_type == 5)
    {
        KPD_air = 0.2;
        j = 1;             //Bicycling
    }
    else
    {
        KPD_air = 0;       //Rest
        j = 0;
    }

    Heat_coef_work = 1 - KPD_air;
    flag_exercise_air = 1;

    for (int i = 0; i < M_Count_Cylinder_14; i++)
    {
        k_ext_air[i] = k_ext_all[j][i];
    }
}
//---------------------------------------------------------------------------

void bioman::Start_New_Exp_0(void)
{
    // Input_Contr_Resp();

    if (t_step == 0)
    {
        Calc_Man();
        Man_start();
        Coef_Skin_Blood_flow_calc();
    }

    // CLOTHES
    if (flag_cloth)
    {
        if (m != m_clothes)
        {
            Clone_Parameters();
            fOpen_FABRIC_current_Csv();
            Man_Clothes();
        }
    }

    SaveEnv();
    Equivalent_he();
    if (flag_immersion) Input_Immersion();

    if (t_step == 0)
    {
        BuilderGraphBP();                           //Forming arrays to display graph
    }
    if (!flag_error_b)
        Bio_Change_Run();
}
//---------------------------------------------------------------------------

void bioman::Save_Table_Results(string save_file)
{
    int i, j;
    char* OldLocale;
    string Line_s;
    string Delimiter_s;
    Delimiter_s = ";";                                                            // Record the table in *.csv format  (Delimiter is semicolon)

    OldLocale = setlocale(LC_NUMERIC, NULL);
    setlocale(LC_NUMERIC, "Russian_Russia.1252");                               // French_Canada.1252

    //***** Titles
    Line_s = ABR_total_gr[0] + (string)", " + Measure_total_gr[0] + Delimiter_s;   //"Time, h"
    Line_s += "T brain, °C" + Delimiter_s;
    Line_s += "T core, °C" + Delimiter_s;                                           //int.org.

    for (j = 1; j < n_total_gr; j++)
    {
        if (Measure_total_gr[j] == "") Line_s += ABR_total_gr[j] + Delimiter_s;
        else
        {
            Line_s += ABR_total_gr[j] + (string)", " + Measure_total_gr[j] + Delimiter_s;
        }
    }
    Line_s += "\n";

    //***** Data
    for (i = 0; i < t_step; i++)
    {
        Line_s += fDouble_to_String(*(*(Total_gr[0]) + i)) + Delimiter_s;              //time
        Line_s += fDouble_to_String(temp[i_man[0][0]][i]) + Delimiter_s;             //brain
        Line_s += fDouble_to_String(temp[i_man[0][1]][i]) + Delimiter_s;             //core

        for (j = 1; j < n_total_gr; j++)
        {
            Line_s += fDouble_to_String(*(*(Total_gr[j]) + i)) + Delimiter_s;
        }
        Line_s += "\n";
    }

    setlocale(LC_NUMERIC, OldLocale);
    fSaving_Results(Line_s, save_file);                                         // Save the table in file
}
//---------------------------------------------------------------------------

void bioman::Android_JSON_Results(string save_results_short)
{
    int time_min1;
    string Line_js_A;
    vector <string> All_Line_A;
    All_Line_A.clear();

    All_Line_A.push_back("{\"dataResult\":{\"Time\":[");                        // 0. Time, min
    All_Line_A.push_back("\"TCore\":[");                                        // 1. Internal organs temperature, °C
    All_Line_A.push_back("\"TSkin\":[");                                        // 2. Mean skin temperature, °C
    All_Line_A.push_back("\"EvapSwRate\":[");                                   // 3. Sweat rate, g/h
    All_Line_A.push_back("\"WL\":[");                                           // 4. Total water loss, g
    All_Line_A.push_back("\"Dripping_sweat\":[");                               // 5. Dripping sweat, g/h
    All_Line_A.push_back("\"Sweat_evaporation\":[");                            // 6. Sweat evaporation, kcal/h (Real_evaporation)        // -----Heart Rate, HR, beats/min
    All_Line_A.push_back("\"Cardiac_output\":[");                               // 7. Cardiac output, CO, l/h
    All_Line_A.push_back("\"Muscles_blood_flow\":[");                           // 8. Muscles blood flow, l/h

    // 0 -- Green zone [36.3 <= core temperature <= 38.5], [water losses <=1.5% of body weight],
    // 1 -- Yellow zone - risk [core temperature <36.3 or >38.5], [water losses >1.5% of body weight],
    // 2 -- Red zone - extreme [core temperature <36 or >39], [water losses >2% of body weight],
    // 3 -- STOP!!! Condition to stop the experiment: [core temperature <35.5 or >39.5], [water losses >4% of body weight].

    All_Line_A.push_back("\"RiskBodyTemp\":[35.5, 36, 36.3, 38.5, 39, 39.5],");
    All_Line_A.push_back("\"RiskWaterLoses\":[2.5, 3, 4],");
    All_Line_A.push_back("\"ID_model\":\"ANDROID\"}}");                          // ID of experiment ***

    time_min1 = 1;                                                                // 1 min (0.01667 h)
    for (int i = 0; i < t_step - 1; time_min1++)
    {
        fLine_Android_Results(All_Line_A, i, ",");
        i = floor(1000 * time_min1 / 60 + 0.5);
    }
    fLine_Android_Results(All_Line_A, t_step - 1, "");
    Line_js_A.clear();
    for (int i = 0; i < 9; i++)
    {
        All_Line_A[i] += "],";
        Line_js_A += All_Line_A[i];                                                // Sending Results to Console
    }
    Line_js_A += All_Line_A[9];
    Line_js_A += All_Line_A[10];
    Line_js_A += All_Line_A[11];
    
    Global_Results = Line_js_A;
    cout << Line_js_A;  
                                                          // Sending Results to Console
    fSaving_Results(Line_js_A, save_results_short);                             // Save the short results in json file
}

//---------------------------------------------------------------------------

void bioman::fLine_Android_Results(vector <string>& All_Line_A, int i, string Line_f1)
{
    int j;
    double buff_WRi;
    buff_WRi = floor(*(*(Total_gr[0]) + i) * 60 + 0.5);
    j = 0;
    All_Line_A[j] += fDouble_to_String(buff_WRi) + Line_f1;                                 // 0. Time, min
    All_Line_A[++j] += fDouble_to_String(temp[i_man[0][1]][i]) + Line_f1;                   // 1. Internal organs temperature, °C
    All_Line_A[++j] += fDouble_to_String(*(*(Total_gr[2]) + i)) + Line_f1;                    // 2. Mean skin temperature, °C
    All_Line_A[++j] += fDouble_to_String(*(*(Total_gr[22]) + i)) + Line_f1;                   // 3. Sweat rate, g/h
    All_Line_A[++j] += fDouble_to_String(*(*(Total_gr[23]) + i)) + Line_f1;                   // 4. Total water loss, g
    All_Line_A[++j] += fDouble_to_String(*(*(Total_gr[21]) + i)) + Line_f1;                   // 5. Dripping sweat, g/h
    All_Line_A[++j] += fDouble_to_String(*(*(Total_gr[20]) + i)) + Line_f1;                   // 6. Sweat evaporation, kcal/h (Real_evaporation)        // -----Heart Rate, HR, beats/min
    All_Line_A[++j] += fDouble_to_String(*(*(Total_gr[7]) + i)) + Line_f1;                    // 7. Cardiac output, CO, l/h
    All_Line_A[++j] += fDouble_to_String(*(*(Total_gr[27]) + i)) + Line_f1;                   // 8. Muscles blood flow, l/h
    // All_Line_A[++j]+=fDouble_to_String(RiskBodyTemp[i])+Line_f1;                          // RiskBodyTemp
    // All_Line_A[++j]+=fDouble_to_String(RiskWaterLoses[i])+Line_f1;                        // RiskWaterLoses
}
//---------------------------------------------------------------------------

void bioman::Android_JSON_Results_WATER(string save_results_short)
{
    int time_min1;
    string Line_js_A;
    vector <string> All_Line_A;
    All_Line_A.clear();

    All_Line_A.push_back("{\"dataResult\":{\"Time\":[");                        // 0. Time, min
    All_Line_A.push_back("\"TCore\":[");                                        // 1. Internal organs temperature, °C
    All_Line_A.push_back("\"TSkin\":[");                                        // 2. Mean skin temperature, °C
    All_Line_A.push_back("\"Skin_blood_flow\":[");                              // 3. Skin blood flow, l/h
    All_Line_A.push_back("\"Heat_flow_via_blood_in_skin\":[");                  // 4. Heat flow via blood in skin, kcal/h
    All_Line_A.push_back("\"Shivering\":[");                                    // 5. Shivering, kcal/h
    All_Line_A.push_back("\"Water_convection\":[");                             // 6. Water convection, kcal/h
    All_Line_A.push_back("\"Cardiac_output\":[");                               // 7. Cardiac output, CO, l/h
    All_Line_A.push_back("\"Muscles_blood_flow\":[");                           // 8. Muscles blood flow, l/h

    // 0 -- Green zone [36.3 <= core temperature <= 38.5], [water losses <=1.5% of body weight],
    // 1 -- Yellow zone - risk [core temperature <36.3 or >38.5], [water losses >1.5% of body weight],
    // 2 -- Red zone - extreme [core temperature <36 or >39], [water losses >2% of body weight],
    // 3 -- STOP!!! Condition to stop the experiment: [core temperature <35.5 or >39.5], [water losses >4% of body weight].

    All_Line_A.push_back("\"RiskBodyTemp\":[35.5, 36, 36.3, 38.5, 39, 39.5],");
    All_Line_A.push_back("\"RiskWaterLoses\":[2.5, 3, 4],");
    All_Line_A.push_back("\"ID_model\":\"ANDROID\"}}");                          // ID of experiment ***

    time_min1 = 1;                                                                // 1 min (0.01667 h)
    for (int i = 0; i < t_step - 1; time_min1++)
    {
        fLine_Android_Results_WATER(All_Line_A, i, ",");
        i = floor(1000 * time_min1 / 60 + 0.5);
    }
    fLine_Android_Results_WATER(All_Line_A, t_step - 1, "");
    Line_js_A.clear();
    for (int i = 0; i < 9; i++)
    {
        All_Line_A[i] += "],";
        Line_js_A += All_Line_A[i];                                                // Sending Results to Console
    }
    Line_js_A += All_Line_A[9];
    Line_js_A += All_Line_A[10];
    Line_js_A += All_Line_A[11];

    cout << Line_js_A;                                                            // Sending Results to Console
    fSaving_Results(Line_js_A, save_results_short);                             // Save the short results in json file
}
//---------------------------------------------------------------------------

void bioman::fLine_Android_Results_WATER(vector <string>& All_Line_A, int i, string Line_f1)
{
    int j;
    double buff_WRi;
    buff_WRi = floor(*(*(Total_gr[0]) + i) * 60 + 0.5);
    j = 0;
    All_Line_A[j] += fDouble_to_String(buff_WRi) + Line_f1;                                 // 0. Time, min
    All_Line_A[++j] += fDouble_to_String(temp[i_man[0][1]][i]) + Line_f1;                   // 1. Internal organs temperature, °C
    All_Line_A[++j] += fDouble_to_String(*(*(Total_gr[2]) + i)) + Line_f1;                    // 2. Mean skin temperature, °C
    All_Line_A[++j] += fDouble_to_String(*(*(Total_gr[26]) + i)) + Line_f1;                   // 3. Skin blood flow, l/h
    All_Line_A[++j] += fDouble_to_String(*(*(Total_gr[11]) + i)) + Line_f1;                   // 4. Heat flow via blood in skin, kcal/h
    All_Line_A[++j] += fDouble_to_String(*(*(Total_gr[28]) + i)) + Line_f1;                   // 5. Shivering, kcal/h
    All_Line_A[++j] += fDouble_to_String(*(*(Total_gr[15]) + i)) + Line_f1;                   // 6. Water convection, kcal/h
    All_Line_A[++j] += fDouble_to_String(*(*(Total_gr[7]) + i)) + Line_f1;                    // 7. Cardiac output, CO, l/h
    All_Line_A[++j] += fDouble_to_String(*(*(Total_gr[27]) + i)) + Line_f1;                   // 8. Muscles blood flow, l/h
    // All_Line_A[++j]+=fDouble_to_String(RiskBodyTemp[i])+Line_f1;                          // RiskBodyTemp
    // All_Line_A[++j]+=fDouble_to_String(RiskWaterLoses[i])+Line_f1;                        // RiskWaterLoses
}
//---------------------------------------------------------------------------

void bioman::Web_JSON_Results(string save_results_short)
{
    int time_min3;
    int count_line;
    string Line_js_W;
    vector <string> All_Line_W;
    All_Line_W.clear();

    All_Line_W.push_back("{\"dataResult\":{\"Time\":[");                        // 0. Time, min
    All_Line_W.push_back("\"TBrain\":[");                                       // 1. Brain temperature, °C
    All_Line_W.push_back("\"TBlood\":[");                                       // 2. Blood temperature, °C
    All_Line_W.push_back("\"TCore\":[");                                        // 3. Internal organs temperature, °C
    All_Line_W.push_back("\"TSkin\":[");                                        // 4. Mean skin temperature, °C
    All_Line_W.push_back("\"TMuscle\":[");                                      // 5. Mean muscle temperature, °C
    All_Line_W.push_back("\"TBody\":[");                                        // 6. Mean body temperature, °C
    All_Line_W.push_back("\"TSkTorso\":[");                                     // 7. Skin torso temperature, °C
    All_Line_W.push_back("\"TSkHands\":[");                                     // 8. Skin hands temperature, °C
    All_Line_W.push_back("\"TSkFeet\":[");                                      // 9. Skin feet temperature, °C
    if (flag_immersion)
    {
        All_Line_W.push_back("\"WaterConv\":[");                                 // 10. Water convection, kcal/h
    }
    else
    {
        All_Line_W.push_back("\"SwEvap\":[");                                    // 10. Sweat evaporation, kcal/h
        All_Line_W.push_back("\"SwRate\":[");                                    // 11. Sweat rate, g/h
        All_Line_W.push_back("\"WL\":[");                                        // 12. Total water loss, g
    }
    All_Line_W.push_back("\"MSh\":[");                                          // 13. Shivering, kcal/h
    All_Line_W.push_back("\"BFsk\":[");                                         // 14. Skin blood flow, l/h
    All_Line_W.push_back("\"BFmus\":[");                                        // 15. Muscle blood flow, l/h
    All_Line_W.push_back("\"CO\":[");                                           // 16. Cardiac output, l/h
    All_Line_W.push_back("\"ID_model\":\"WEB\"}");                              // ID of experiment ***

    count_line = All_Line_W.size();

    time_min3 = 1;                                                                // 1 min (0.01667 h)
    for (int i = 0; i < t_step - 1; time_min3++)
    {
        fLine_Web_Results(All_Line_W, i, ",");
        i = floor(1000 * time_min3 / 60 + 0.5);
    }
    fLine_Web_Results(All_Line_W, t_step - 1, "");
    Line_js_W.clear();
    for (int i = 0; i < count_line - 2; i++)
    {
        All_Line_W[i] += "],";
        Line_js_W += All_Line_W[i];                                                // Sending Results to Console
    }
    All_Line_W[count_line - 2] += "]},";
    Line_js_W += All_Line_W[count_line - 2];
    Line_js_W += All_Line_W[count_line - 1];

    cout << Line_js_W;
    fSaving_Results(Line_js_W, save_results_short);                             // Save the short results in json file
}
//---------------------------------------------------------------------------

void bioman::fLine_Web_Results(vector <string>& All_Line_W, int i, string Line_f2)
{
    int j;
    double buff_WRi;
    buff_WRi = floor(*(*(Total_gr[0]) + i) * 60 + 0.5);
    j = 0;
    All_Line_W[j] += fDouble_to_String(buff_WRi) + Line_f2;                         // 0. Time, min
    All_Line_W[++j] += fDouble_to_String(temp[i_man[0][0]][i]) + Line_f2;           // 1. Brain temperature, °C
    All_Line_W[++j] += fDouble_to_String(*(*(Total_gr[1]) + i)) + Line_f2;            // 2. Blood temperature, °C
    All_Line_W[++j] += fDouble_to_String(temp[i_man[0][1]][i]) + Line_f2;           // 3. Internal organs temperature, °C
    All_Line_W[++j] += fDouble_to_String(*(*(Total_gr[2]) + i)) + Line_f2;            // 4. Mean skin temperature, °C
    All_Line_W[++j] += fDouble_to_String(*(*(Total_gr[4]) + i)) + Line_f2;            // 5. Mean muscle temperature, °C
    All_Line_W[++j] += fDouble_to_String(*(*(Total_gr[3]) + i)) + Line_f2;            // 6. Mean body temperature, °C
    All_Line_W[++j] += fDouble_to_String(temp[i_man[1][1]][i]) + Line_f2;           // 7. Skin torso temperature, °C

    buff_WRi = (temp[i_man[1][6]][i] + temp[i_man[1][7]][i]) / 2;
    buff_WRi = floor(buff_WRi * 1000 + 0.5) / 1000;
    All_Line_W[++j] += fDouble_to_String(buff_WRi) + Line_f2;                       // 8. Skin hands temperature, °C

    buff_WRi = (temp[i_man[1][12]][i] + temp[i_man[1][13]][i]) / 2;
    buff_WRi = floor(buff_WRi * 1000 + 0.5) / 1000;
    All_Line_W[++j] += fDouble_to_String(buff_WRi) + Line_f2;                       // 9. Skin feet temperature, °C

    if (flag_immersion)
    {
        All_Line_W[++j] += fDouble_to_String(*(*(Total_gr[15]) + i)) + Line_f2;        // 10. Water convection, kcal/h
    }
    else
    {
        All_Line_W[++j] += fDouble_to_String(*(*(Total_gr[20]) + i)) + Line_f2;        // 10. Sweat evaporation, kcal/h
        All_Line_W[++j] += fDouble_to_String(*(*(Total_gr[22]) + i)) + Line_f2;        // 11. Sweat rate, g/h
        All_Line_W[++j] += fDouble_to_String(*(*(Total_gr[23]) + i)) + Line_f2;        // 12. Total water loss, g
    }

    All_Line_W[++j] += fDouble_to_String(*(*(Total_gr[28]) + i)) + Line_f2;           // 13. Shivering, kcal/h
    All_Line_W[++j] += fDouble_to_String(*(*(Total_gr[26]) + i)) + Line_f2;           // 14. Skin blood flow, l/h
    All_Line_W[++j] += fDouble_to_String(*(*(Total_gr[27]) + i)) + Line_f2;           // 15. Muscle blood flow, l/h
    All_Line_W[++j] += fDouble_to_String(*(*(Total_gr[7]) + i)) + Line_f2;            // 16. Cardiac output, l/h
}
//---------------------------------------------------------------------------

void bioman::fSaving_Exp_Cond_Json(string save_file_js_ec)
{
    int i, time_min5;
    string Line_jsec;

    Line_jsec = "{\"Experimental_Conditions\":{\"Time_start\":" + fDouble_to_String(t_ex_start);
    time_min5 = floor(t_ex_finish * 60 + 0.5);
    Line_jsec += ",\"Time_finish\":" + fInt_to_String(time_min5);                   // Time, min

    if (flag_exercise_air > 0)
    {
        Line_jsec += ",\"Qexercise_air\":" + fInt_to_String(Qe_air);                 // Qexercise, kcal/h - physical activity (load, exercise)
        Line_jsec += ",\"V_activity_air\":" + fDouble_to_String(v_activity_air);
        Line_jsec += ",\"KPD_air\":" + fDouble_to_String(KPD_air);
        Line_jsec += ",\"Temperature_air\":" + fDouble_to_String(total_ta);
        Line_jsec += ",\"RH_air\":" + fDouble_to_String(total_rh);
        Line_jsec += ",\"Velocity_air\":";
        Line_jsec += Values_js[7].c_str();
    }
    if (flag_water > 0)
    {
        Line_jsec += ",\"Qexercise_water\":" + fInt_to_String(Qe_water);
        Line_jsec += ",\"KPD_water\":" + fDouble_to_String(KPD_water);
        Line_jsec += ",\"Temperature_water\":" + fDouble_to_String(temp_water);
        Line_jsec += ",\"Temperature_air_imm\":" + fDouble_to_String(t_air_imm);
        Line_jsec += ",\"RH_air_imm\":" + fDouble_to_String(rh_air_imm);
        Line_jsec += ",\"Velocity_air_imm\":" + fDouble_to_String(v_air_imm);
        Line_jsec += ",\"Immersion_level\":[\"" + fInt_to_String(Water_Level[0]);
        for (i = 1; i < M_Count_Cylinder_14; i++)
            Line_jsec += "\",\"" + fInt_to_String(Water_Level[i]);
        Line_jsec += "\"]";
    }
    Line_jsec += ",\"Clothes\":[\"" + fInt_to_String(Cloth_code[0]);
    for (i = 1; i < M_Count_Cylinder_14; i++)
        Line_jsec += "\",\"" + fInt_to_String(Cloth_code[i]);
    Line_jsec += "\"]";


    Line_jsec += "},\"modelParameters\":{\"type_activities\":";
    Line_jsec += Values_js[1].c_str();
    Line_jsec += ",\"sub_type_activities\":";
    Line_jsec += Values_js[2].c_str();
    Line_jsec += "}}";
    fSaving_Results(Line_jsec, save_file_js_ec);                                // Save the experimental conditions in json file
}
//---------------------------------------------------------------------------

void bioman::fSaving_Results(string& Text_Res, string Path_Res)
{
    FILE* File_Res;
    fopen_s(&File_Res,Path_Res.c_str(), "w");
    fprintf(File_Res, "%s", Text_Res);
    fclose(File_Res);
}
//---------------------------------------------------------------------------

string bioman::fDouble_to_String(double f_w)
{
    string Line_f3;
    char Line_f4[String_Convert_Size];
    snprintf(Line_f4, String_Convert_Size, "%g", f_w);
    Line_f3.clear();
    Line_f3 = Line_f4;
    return Line_f3;
}
//---------------------------------------------------------------------------

string bioman::fInt_to_String(int i_w)
{
    string Line_f5;
    char Line_f6[String_Convert_Size];
    snprintf(Line_f6, String_Convert_Size, "%d", i_w);
    Line_f5.clear();
    Line_f5 = Line_f6;
    return Line_f5;
}
//---------------------------------------------------------------------------

double bioman::fString_to_Double(string Line_3)
{
    int i;
    if ((i = Line_3.find(",")) != string::npos)
        Line_3.replace(i, 1, ".");

    return atof(Line_3.c_str());
}
//---------------------------------------------------------------------------

