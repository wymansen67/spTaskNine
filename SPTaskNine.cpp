﻿#include <iostream>
#include <windows.h>
using namespace std;

struct ThreadArgs {
    int* array;
    int begin;
    int end;
};

// Merges two subarrays of array[].
void merge(int array[], int const left, int const mid, int const right) {
    int const subArrayOne = mid - left + 1;
    int const subArrayTwo = right - mid;

    int* leftArray = new int[subArrayOne];
    int* rightArray = new int[subArrayTwo];

    for (int i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (int j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    int indexOfSubArrayOne = 0;
    int indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;

    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }

    delete[] leftArray;
    delete[] rightArray;
}

DWORD WINAPI mergeSortThread(LPVOID lpParam) {
    ThreadArgs* args = reinterpret_cast<ThreadArgs*>(lpParam);
    int* array = args->array;
    int begin = args->begin;
    int end = args->end;

    if (begin < end) {
        int mid = begin + (end - begin) / 2;

        HANDLE leftThread = CreateThread(NULL, 0, mergeSortThread, new ThreadArgs{ array, begin, mid }, 0, NULL);
        HANDLE rightThread = CreateThread(NULL, 0, mergeSortThread, new ThreadArgs{ array, mid + 1, end }, 0, NULL);

        if (leftThread != NULL && rightThread != NULL) {
            WaitForSingleObject(leftThread, INFINITE);
            WaitForSingleObject(rightThread, INFINITE);

            merge(array, begin, mid, end);

            CloseHandle(leftThread);
            CloseHandle(rightThread);
        }
    }

    delete args;
    return 0;
}

void printArray(int A[], int size) {
    for (auto i = 0; i < size; i++)
        cout << A[i] << " ";
    cout << endl;
}

int main() {
    INT arr[] = { 793, 3394, 2092, 4724, 2783, 1086, 4532, 3157, 4397, 4913, 1590, 4117, 2434, 3057, 1009, 3663, 1977, 3923, 3283, 4827, 1531, 1606, 4963, 3944, 3532, 4934, 1990, 4551, 2355, 3637, 2683, 1341, 4681, 3321, 2649, 2107, 2224, 3942, 4586, 2655, 1048, 3652, 1267, 1126, 2214, 2492, 2060, 2507, 1911, 2487, 3721, 2071, 3858, 1500, 3530, 2503, 1174, 4966, 1647, 3348, 1204, 4970, 2258, 1166, 1748, 4122, 4842, 4903, 2231, 2899, 1971, 2386, 4683, 3145, 2181, 4020, 2935, 4798, 3434, 4781, 3640, 1308, 4503, 3591, 2715, 2674, 3451, 2043, 3067, 3531, 1757, 4237, 4892, 4653, 4465, 3963, 4177, 1756, 3195, 3671, 4553, 2522, 1422, 1004, 4271, 3797, 3341, 1924, 2927, 1353, 1994, 1285, 2615, 1785, 4304, 2838, 3566, 1319, 3455, 1575, 2795, 3890, 3912, 2908, 2343, 4258, 3761, 4375, 1402, 4621, 3964, 4906, 1221, 3641, 2657, 4697, 4626, 2252, 2618, 4515, 3424, 2123, 2914, 3204, 4024, 3747, 1929, 1868, 4533, 2277, 4449, 2572, 3331, 4567, 1015, 1343, 4876, 3084, 4171, 4589, 4470, 1867, 3483, 2858, 3179, 4408, 1271, 2873, 3835, 2817, 2411, 4521, 4638, 1026, 3962, 4801, 3658, 3207, 3432, 4211, 4762, 1923, 2880, 1210, 2073, 4857, 2761, 2559, 1982, 4432, 4673, 4026, 1090, 2789, 2216, 1797, 2392, 4087, 1602, 2549, 1681, 3274, 4924, 4712, 4462, 4434, 4965, 4021, 3325, 3833, 2971, 4345, 4453, 2941, 2672, 2380, 2254, 3969, 3304, 1008, 1788, 3153, 4971, 3081, 2782, 1991, 4077, 3489, 3879, 3200, 2541, 2749, 1167, 2731, 2069, 4862, 3456, 3834, 2945, 4380, 2068, 1102, 3408, 4422, 4334, 4362, 2669, 2422, 3070, 2640, 2849, 4911, 4872, 1574, 2293, 3282, 2740, 2011, 3318, 3533, 4016, 2025, 1241, 2814, 2247, 1218, 2275, 2299, 4401, 3158, 1324, 4672, 1661, 4641, 1918, 2164, 4435, 4579, 3741, 3421, 4173, 2064, 4771, 1375, 1372, 2128, 2437, 1073, 2200, 4714, 3687, 3062, 3752, 4028, 1461, 1160, 2238, 3285, 4741, 1817, 2327, 4580, 3246, 4471, 4701, 4581, 1578, 1981, 2588, 2800, 3437, 1237, 4631, 3788, 4138, 2132, 2360, 2358, 1774, 2563, 4015, 3256, 1488, 3251, 3163, 1763, 3744, 4494, 1905, 2987, 3352, 3344, 4732, 4476, 1128, 4718, 2784, 4883, 4957, 2219, 2066, 1988, 1576, 4442, 1701, 4737, 3803, 1322, 1927, 4846, 4975, 4153, 3759, 3862, 2778, 4822, 2585, 2397, 1831, 1185, 2573, 3416, 1201, 2991, 4051, 3241, 1582, 2183, 1395, 2210, 1094, 3104, 3485, 1321, 3732, 1697, 3012, 4547, 2557, 2501, 3410, 3159, 2114, 1029, 1186, 4414, 3063, 1540, 1454, 4337, 3769, 2870, 4223, 2545, 4501, 4509, 2354, 3864, 3988, 3295, 2662, 3980, 3673, 3244, 3473, 4899, 3327, 4287, 4524, 4623, 2999, 1627, 4823, 2030, 3892, 2872, 1299, 2168, 2768, 2185, 4130, 2843, 2772, 2978, 3830, 2596, 4594, 2822, 2536, 2146, 1110, 1486, 1394, 1261, 2120, 2819, 4936, 3891, 3252, 1103, 2110, 2665, 2950, 2464, 1520, 2465, 3138, 4498, 2654, 1844, 4646, 4169, 1251, 4777, 3665, 1719, 3328, 4456, 1429, 2871, 2248, 2149, 1335, 4119, 2884, 3412, 1690, 1917, 3929, 2664, 4192, 4336, 3577, 4226, 2540, 1344, 3335, 4214, 4874, 2369, 2488, 3601, 1518, 2850, 1487, 2111, 4734, 2245, 3140, 3482, 3125, 1665, 2763, 2979, 1024, 2211, 1897, 3329, 4455, 3729, 2897, 1635, 3174, 2555, 2163, 2202, 4721, 3725, 4131, 3679, 4973, 3792, 1940, 1130, 3869, 4709, 2160, 1154, 2266, 3754, 4620, 2173, 4373, 3993, 3794, 3484, 3826, 3105, 2336, 1145, 3822, 4571, 4390, 1686, 2194, 2729, 4203, 3486, 3998, 2013, 1217, 3856, 2785, 2415, 2891, 3922, 2348, 1348, 4037, 4542, 2954, 4317, 1312, 1900, 1547, 4350, 1594, 2690, 1410, 1328, 2085, 2046, 1747, 3099, 3724, 3151, 1071, 4706, 2972, 2764, 4218, 3142, 4296, 1302, 2974, 2368, 3452, 4396, 2137, 1109, 2574, 4292, 3209, 2081, 3657, 4111, 1818, 3710, 3984, 4266, 3098, 4730, 2808, 2617, 3538, 2067, 1626, 2256, 2344, 2291, 1380, 4848, 2824, 3844, 1164, 4624, 1790, 1705, 1313, 2753, 1156, 2821, 1070, 2767, 4935, 3965, 3723, 3101, 1895, 1281, 2813, 1485, 3865, 4545, 1147, 3240, 4314, 4558, 1303, 4725, 1567, 1610, 1138, 3795, 2684, 3286, 1493, 2417, 1585, 4215, 4956, 2656, 2520, 2246, 1283, 2395, 4535, 3497, 4313, 2141, 1841, 4163, 3077, 1262, 4544, 3085, 2458, 2340, 2966, 1634, 4438, 1010, 3677, 3521, 3799, 2184, 4327, 1187, 2167, 2145, 1131, 1768, 1710, 3534, 2878, 1728, 4967, 4588, 4518, 2832, 3784, 4691, 3863, 4783, 3255, 2148, 3154, 3005, 4180, 3781, 4058, 1006, 4377, 4251, 4764, 1800, 3425, 2993, 4175, 3141, 1630, 1933, 1377, 1928, 4866, 1723, 3875, 4943, 3210, 3025, 2820, 4531, 4417, 4463, 3768, 2263, 3779, 1072, 2895, 2041, 4510, 2284, 1229, 2243, 4033, 3152, 2083, 1300, 3893, 1620, 3796, 3418, 2679, 3808, 1808, 4154, 4480, 2760, 2398, 4854, 1930, 2827, 3775, 1195, 1954, 3527, 4441, 1144, 4371, 4340, 3423, 4231, 1417, 4010, 3402, 2294, 1012, 2868, 4166, 1401, 3109, 3367, 1516, 4398, 3708, 3292, 2169, 4293, 1161, 3442, 1371, 4585, 2746, 2809, 4665, 4564, 3910, 4760, 2178, 3668, 1438, 3739, 1832, 4236, 1198, 4468, 2383, 4075, 4664, 3873, 1775, 2787, 4457, 1301, 4645, 4262, 4508, 3059, 3836, 1152, 3022, 1055, 1266, 4248, 4728, 2215, 3224, 1228, 4817, 3018, 2580, 4763, 1639, 4234, 4445, 1623, 2678, 3660, 2773, 1027, 2942, 4603, 1211, 3750, 2288, 1244, 1636, 3061, 1731, 3415, 4297, 3477, 3720, 3896, 4622, 4264, 1904, 4828, 1496, 1079, 3946, 4530, 2416, 3610, 2652, 1996, 3919, 2267, 1694, 2391, 3828, 4444, 2088, 1984, 4787, 1890, 4496, 1510, 2217, 1632, 1715, 1290, 2341, 1579, 4525, 4694, 1253, 2313, 4098, 3448, 3589, 1724, 2700, 2412, 3586, 1762, 2432, 4492, 3015, 2909, 1951, 4552, 3561, 4644, 1537, 4550, 1293, 1214, 2349, 4250, 4860, 4742, 1742, 3439, 2775, 3373, 1398, 4459, 1907, 1075, 1556, 3362, 4458, 1586, 3866, 1919, 1250, 3453, 3839, 1351, 4186, 1080, 3343, 4794, 1784, 2722, 3676, 3540, 1242, 1814, 1983, 2441, 4381, 3135, 1735, 2644, 3349, 3585, 1074, 3819, 2289, 1104, 2329, 2140, 2062, 3642, 3369, 2350, 3223, 4790, 2710, 1588, 4584, 3083, 4246, 3520, 1101, 3838, 4719, 3370, 2292, 1426, 3753, 4820, 3845, 2166, 4958, 4663, 1712, 3802, 2031, 1993, 1530, 2969, 1035, 3187, 4174, 2881, 3391, 2744, 2892, 1989, 2044, 3743, 1568, 1180, 1503, 3911, 1370, 4146, 4656, 4151, 2742, 3215, 1254, 2745, 3147, 1542, 3401, 2469, 1252, 1151, 3600, 1711, 1767, 2278, 2179, 1746, 1472, 3756, 2566, 1512, 1570, 2823, 4335, 2837, 1378, 4557, 3281, 1484, 1165, 3780, 1673, 4081, 2394, 2913, 3245, 1157, 2094, 3818, 2905, 4675, 3353, 1431, 4233, 2509, 4504, 2225, 3156, 4563, 2061, 3313, 4454, 4630, 3852, 3033, 4439, 4141, 2080, 3598, 2312, 2856, 2213, 2138, 4349, 2658, 1338, 2874, 1852, 1112, 4628, 3793, 3804, 1121, 3604, 4083, 4213, 4254, 4972, 3488, 1337, 4655, 4774, 1718, 1219, 1334, 3548, 3143, 1169, 2143, 4158, 3487, 4548, 1628, 2346, 4727, 3718, 1513, 2295, 3493, 1672, 1752, 1527, 4299, 2230, 3983, 3662, 1036, 2016, 4699, 4269, 2514, 3054, 4907, 3127, 4452, 2755, 4178, 1025, 1436, 2121, 3872, 1255, 2191, 2135, 3011, 2890, 4556, 3678, 3672, 3981, 1307, 3360, 3697, 1040, 1359, 1754, 1060, 4239, 1558, 1339, 1463, 3220, 3206, 3842, 4385, 4649, 4294, 1960, 3535, 3226, 1424, 3123, 1437, 2082, 1323, 1501, 3492, 3713, 3690, 1604, 4172, 2867, 3193, 1966, 3013, 4221, 3494, 3887, 2251, 2345, 2381, 3276, 2982, 3667, 4324, 4368, 1032, 2902, 1265, 3381, 1037, 1355, 3647, 3438, 4968, 1751, 2677, 1379, 1450, 1054, 1354, 4003, 2741, 2484, 1414, 1538, 1865, 3371, 3528, 3961, 1663, 2142, 4450, 2042, 1745, 4708, 4797, 3042, 1458, 2877, 1183, 1835, 1689, 1741, 2826, 1909, 2188, 1922, 1408, 4788, 1687, 1716, 3273, 4639, 3185, 3383, 3491, 1819, 2988, 3656, 4376, 1148, 3358, 2554, 2780, 4770, 1544, 1906, 1987, 1225, 4959, 2014, 2516, 3820, 1085, 4599, 3968, 3149, 3790, 1963, 4950, 1720, 4179, 3259, 3480, 2065, 3316, 4816, 1949, 2260, 2244, 4561, 4912, 4093, 4370, 2447, 3730, 1901, 4625, 1333, 3616, 3120, 4360, 4303, 2282, 2356, 4716, 3582, 3860, 3967, 2147, 2943, 3897, 2174, 4517, 4253, 3898, 4418, 2543, 4637, 4466, 2696, 2517, 4255, 1926, 1631, 2786, 3464, 3284, 3580, 3920, 1105, 1925, 3334, 1863, 2334, 3850, 4319, 1455, 4520, 2187, 1504, 3082, 1423, 4526, 3137, 1633, 1802, 3036, 4702, 3150, 4939, 3472, 2374, 3960, 2893, 2063, 4367, 1985, 3249, 4647, 3945, 2769, 1629, 1507, 4680, 2235, 4819, 2489, 3926, 2829, 3463, 4164, 3106, 3218, 4668, 3617, 3049, 1231, 1057, 2410, 1968, 3146, 2339, 1589, 1859, 2218, 1492, 2898, 2904, 3459, 3065, 2133, 3010, 1199, 4780, 1944, 2335, 1215, 4713, 1866, 1667, 2584, 4447, 3388, 1874, 2879, 1021, 2414, 3169, 4522, 3124, 3131, 1013, 1833, 3368, 3162, 4778, 3764, 4386, 1358, 1708, 2463, 3347, 3925, 2485, 1087, 1077, 2223, 1760, 1608, 1081, 3694, 3915, 4054, 4964, 4577, 1709, 4512, 1842, 1163, 3359, 1961, 3029, 1713, 3380, 1190, 3078, 3144, 4150, 3584, 2725, 4710, 1475, 4597, 3859, 2022, 4601, 1159, 2130, 1678, 1273, 4070, 1811, 1031, 3481, 1535, 2331, 1474, 3278, 1286, 1858, 1494, 3899, 2788, 3107, 3587, 3287, 4685, 2036, 4013, 1310, 2045, 1078, 3544, 3870, 2876, 3003, 1903, 2975, 3026, 1162, 2682, 2737, 1543, 4527, 2396, 3787, 2925, 2586, 1376, 4252, 1007, 2429, 1477, 3762, 1854, 1405, 2165, 2212, 3621, 3289, 1069, 3288, 4944, 1017, 4629, 1274, 4232, 2373, 2527, 2732, 2298, 2468, 4448, 4018, 3525, 3623, 1685, 2318, 4679, 1318, 4690, 1264, 3214, 1489, 4516, 1583, 2668, 2428, 1205, 4565, 4451, 1920, 2481, 4693, 2929, 2214, 2162, 2249, 3588, 4383, 4284, 1098, 3173, 4145, 2970, 2712, 1076, 2315, 1224, 4464, 4160, 3041, 4312, 1018, 1821, 3148, 1447, 1603, 3384, 1413, 1399, 2973, 1175, 4311, 1062, 1292, 3034, 4937, 4025, 2875, 4181, 1049, 2426, 4165, 2922, 1896, 3478, 4705, 1388, 3102, 3669, 1451, 4758, 2743, 4636, 2739, 4043, 3228, 3337, 4941, 1816, 1646, 1382, 1093, 4559, 2691, 4481, 1411, 2236, 4410, 1124, 1155, 2663, 3017, 4917, 3315, 4473, 2460, 4238, 2152, 1047, 4657, 4389, 4431, 3190, 1725, 1001, 2379, 4791, 2452, 1862, 4765, 4048, 1820, 2196, 3050, 2445, 3038, 2748, 4918, 2547, 4059, 1732, 1525, 3299, 1446, 2719, 1969, 3413, 1986, 4332, 1974, 1129, 3483, 2747, 1390, 3122, 3208, 1239, 2734, 2390, 1373, 1467, 4265, 2078, 1786, 4443, 3627, 4295, 3048, 3666, 3693, 1111, 2702, 1592, 2546, 1304, 2727, 2907, 2419, 2201, 4652, 2794, 2064, 1083, 3409, 2017, 4575, 3791, 4952, 4511, 3222, 1980, 1068, 4039, 3475, 1282, 4602, 3450, 4361, 3867, 1766, 4539, 4538, 3374, 4587, 3661, 4951, 2815, 1243, 3467, 3182, 1272, 1805, 4322, 4092, 4488, 1734, 2587, 4505, 2333, 3155, 4142, 2828, 2032, 4205, 2467, 4930, 2281, 4720, 2115, 3659, 3465, 3145, 4768, 2855, 3466, 3664, 4627, 2321, 3047, 4382, 1580, 4704, 1070, 4115, 2189, 2680, 1572, 3805, 1278, 1587, 3333, 3387, 3291, 4772, 2483, 2707, 3894, 2864, 1457, 3645, 3829, 4355, 1468, 4369, 2944, 4210, 2099, 2553, 2048, 3564, 1234, 2781, 3648, 1740, 2552, 3755, 4286, 4851, 2896, 1528, 4341, 3243, 2782, 3692, 3698, 2928, 2205, 1521, 3429, 1936, 2232, 2801, 2399, 4931, 2040, 4864, 4187, 3851, 1948, 4406, 4268, 3757, 1089, 4753, 1997, 3213, 1959, 1577, 2387, 3861, 4235, 2611, 3814, 4870, 2172, 1958, 3134, 4634, 2948, 4469, 2687, 2482, 2883, 4162, 3696, 1765, 4537, 2253, 2685, 4456, 1642, 1815, 1289, 3999, 4591, 1497, 2283, 1084, 2818, 4430, 4056, 4168, 2577, 3765, 3435, 4871, 1361, 4184, 3280, 2980, 3247, 1082, 2686, 1807, 2789, 4715, 2314, 2122, 4378, 2091, 3128, 3242, 3108, 1227, 2953, 1607, 1983, 1434, 4230, 1695, 4722, 2002, 1332, 4364, 1573, 4782, 3884, 1260, 2756, 2771, 4419, 2241, 4090, 1499, 4233, 3019, 3097, 2558, 3666, 2670, 3053, 4170, 2998, 2977, 4974, 3798, 2480, 3482, 2681, 2352, 1955, 1519, 1755, 1553, 4333, 2164, 4744, 1172, 2385, 3811, 2363, 2595, 2649, 4712, 1971, 4019, 2221, 1439, 1714, 1935, 2754, 4666, 1982, 1342, 4401, 1677, 4540, 4766, 2139, 3886, 1998, 1562, 4465, 2683, 3986, 4095, 2761, 3468, 1374, 1232, 3348, 2899, 1582, 1619, 2060, 3126, 1534, 3350, 3566, 3163, 3427, 3172, 1306, 3052, 2650, 3995, 1212, 2674, 1849, 4519, 1726, 4486, 1749, 4633, 2604, 4216, 2997, 3626, 4607, 1526, 2733, 1541, 4506, 4754, 2588, 2092, 4256, 4631, 1073, 2926, 3297, 1878, 2706, 2144, 4604, 2161, 3499, 3772, 2069, 4515, 4701, 4502, 1546, 3411, 1788, 1091, 4595, 2889, 1221, 4031, 2923, 2873, 3460, 4358, 3640, 1051, 2976, 4875, 2190, 2028, 1005, 3044, 1500, 3490, 3928, 4606, 1146, 1517, 1584, 2796, 1921, 2797, 2128, 1249, 3985, 4351, 4752, 3788, 4182, 1412, 4863, 2952, 1817, 3691, 1945, 3825, 3841, 1079, 3096, 3695, 2286, 2762, 3880, 3057, 3436, 2392, 1559, 2043, 1967, 3832, 2171, 1602, 1524, 3663, 4579, 4543, 3767, 4036, 2908, 2117, 1853, 2924, 1132, 2564, 1905, 3581, 2100, 3086, 2572, 1206, 1845, 1522, 4711, 2968, 3324, 1088, 4651, 4626, 3364, 2129, 1679, 4112, 4810, 3404, 1402, 4339, 4867, 2127, 4938, 4507, 2367, 1367, 1894, 2971, 3763, 3489, 2825, 2857, 3797, 3365, 4714, 1555, 4161, 4653, 1880, 2602, 2216, 4528, 1403, 3485, 4536, 3397, 1153, 3207, 2640, 2423, 3212, 1372, 1722, 1560, 3121, 2210, 4562, 2749, 3680, 3089, 1061, 2740, 3142, 4240, 2198, 2431, 4259, 3547, 2537, 2885, 3990, 4263, 3270, 4440, 1550, 3204, 3469, 1688, 1167, 4605, 3246, 1876, 2544, 4420, 3395, 4387, 1965, 2735, 1022, 1203, 3699, 2814, 2713, 2293, 3644, 2270, 2401, 1717, 2894, 4267, 2799, 3824, 4021, 3785, 3031, 2530, 1118, 1181, 3045, 2559, 3803, 4331, 1621, 4773, 3129, 2186, 1941, 1294, 4118, 2812, 1637, 2791, 1331, 2250, 2757, 3766, 3658, 1851, 2343, 1350, 2073, 1748, 4416, 1812, 2109, 1574, 3191, 3248, 1891, 3461, 3877, 2446, 1843, 1392, 2134, 2985, 1430, 1391, 2870, 1848, 3363, 4485, 2919, 2779, 4700, 2940, 1482, 4436, 3476, 2181, 1964, 1053, 3782, 2945, 1662, 3833, 4217, 2984, 2420, 4723, 1609, 3056, 3498, 3793, 1946, 4411, 1014, 2551, 2900, 2183, 4547, 2119, 2630, 1871, 1343, 3665, 2026, 1898, 4094, 4697, 2149, 1850, 4372, 4261, 2740, 3296, 2871, 4574, 2411, 1349, 4424, 3827, 3745, 1460, 2821, 2784, 2168, 4635, 1759, 2146, 1267, 3471, 2219, 1660, 1056, 3526, 3040, 4483, 1886, 2397, 2093, 3189, 1520, 3221, 4643, 2368, 4487, 2785, 1539, 4207, 4287, 4789, 3209, 1229, 4609, 4524, 3183, 3962, 1230, 1900, 2316, 2231, 1131, 4762, 1197, 3198, 4149, 4049, 1804, 2507, 4338, 1877, 4243, 1928, 3881, 2332, 3994, 1567, 4148, 1611, 1970, 1319, 1459, 2614, 3151, 1892, 1547, 3300, 4953, 3087, 1744, 4865, 1595, 3806, 4844, 3479, 3179, 2229, 1753, 1284, 3175, 2240, 1410, 3980, 3789, 2311, 4620, 3698, 1658, 2211, 4461, 2850, 2434, 2853, 4336, 4776, 4260, 1044, 1486, 2180, 4474, 1743, 4470, 4225, 4914, 1365, 4266, 1918, 1223, 4033, 4661, 1511, 3657, 4045, 3392, 2783, 2512, 4237, 4600, 3303, 4608, 1326, 2443, 2450, 1071, 3293, 1120, 3916, 4659, 2224, 1298, 1823, 1196, 3219, 4602, 2421, 4163, 2939, 4943, 3741, 1638, 3796, 3088, 4785, 2417, 1262, 4523, 3030, 4777, 1033, 1184, 2000, 1065, 3565, 4244, 2255, 1024, 4829, 1545, 2228, 2175, 2872, 2738, 4345, 4650, 3275, 3470, 2583, 1019, 4438, 1043, 3847, 1397, 1233, 4476, 1443, 1109, 1787, 4037, 4501, 1208, 1066, 4976, 2897, 2125, 1810, 3455, 1191, 1086, 2081, 3174, 3068, 3458, 2209, 3344, 4352, 4314, 1235, 4717, 3014, 1763, 3283, 1758, 1357, 1885, 3601, 3168, 3240, 2906, 1345, 1409, 2245, 3319, 4001, 1593, 3721, 1483, 1962, 2792, 2777, 1561, 2660, 3662, 4500, 1377, 2532, 3484, 2723, 2538, 1943, 2380, 2502, 3192, 4598, 4954, 1523, 4407, 4795, 3095, 3462, 2736, 4660, 1245, 4792, 2664, 4796, 2279, 1327, 3800, 3227, 2067, 3486, 4747, 2671, 2887, 3457, 4811, 3641, 3495, 4408, 1020, 2774, 2642, 1676, 4913, 3157, 4267, 3136, 2585, 4624, 1026, 3310, 4945, 4589, 2772, 3153, 3799, 3948, 1103, 2767, 1626, 3667, 4868, 4052, 3141, 3758, 4544, 3285, 3654, 1889, 3685, 1461, 1940, 1964, 1400, 3670, 2795, 1999, 2192, 3747, 3328, 4654, 1072, 4114, 3271, 2684, 2275, 4691, 4623, 4374, 4428, 3671, 3167, 1768, 3643, 4412, 2297, 1083, 4176, 2991, 3225, 2659, 4572, 1297, 3164, 2277, 1981, 2170, 1095, 2965, 2766, 3496, 1096, 3771, 1119, 1193, 3432, 3808, 2711, 2415, 3778, 2903, 3414, 4823, 2204, 1296, 1473, 3012, 4509, 1576, 2090, 1650, 1322, 1226, 1325, 1887, 1134, 3375, 1844, 2163, 4969, 4934, 1747, 3197, 4578, 2185, 4449, 4549, 4110, 3783, 2365, 4258, 1045, 1813, 2430, 2359, 1240, 4241, 3254, 1222, 1746, 2487, 1094, 3784, 2182, 2425, 3681, 1554, 1600, 2290, 4290, 3947, 1335, 3099, 3918, 1416, 2651, 1881, 3152, 2891, 1675, 2729, 1324, 2154, 3158, 3180, 3813, 4442, 3919, 1207, 1092, 1683, 3170, 4046, 1761, 4860, 3223, 1907, 1097, 2242, 4531, 1268, 2840, 1727, 2123, 4557, 1052, 1383, 3852, 3780, 1375, 3989, 4380, 4621, 3462, 2579, 1984, 2746, 2116, 1090, 2085, 3199, 1055, 3532, 4224, 3177, 4455, 1797, 1317, 2901, 4669, 1568, 3750, 2280, 4057, 4585, 1496, 3792, 2515, 3879, 3653, 4632, 2004, 4794, 2213, 2292, 4508, 1122, 2077, 3125, 2357, 2044, 3037, 3370, 2358, 2003, 1110, 2264, 4015, 1668, 1902, 2770, 1527, 3773, 2561, 1529, 2084, 4530, 3166, 1309, 3540, 3274, 3542, 1552, 2355, 1933, 1059, 4554, 4645, 1993, 3816, 2341, 4262, 2938, 3069, 1672, 2679, 4113, 1115, 2816, 2705, 4429, 3776, 3281, 1209, 1681, 1581, 3624, 2720, 2888, 3206, 2949, 3440, 1236, 1498, 2222, 4817, 2874, 3463, 1369, 1348, 2438, 1666, 2354, 1777, 1275, 4317, 3362, 3826, 1320, 2132, 1437, 1979, 1719, 1360, 2177, 1075, 2413, 1271, 1856, 4359, 2969, 3869, 2461, 1531, 2148, 3286, 1340, 1080, 3123, 1657, 3431, 2268, 2457, 4363, 4728, 3611, 1508, 2361, 1290, 1671, 2805, 3064, 3821, 1852, 4655, 3497, 2573, 1973, 4774, 4415, 2617, 4799, 2576, 2015, 1557, 2835, 3610, 3768, 3722, 2217, 4793, 4362, 2406, 3723, 4576, 2504, 1890, 2220, 3801, 4379, 1293, 1064, 2722, 3338, 1287, 1578, 2549, 1237, 1972, 2708, 4421, 3893, 2721, 1951, 1692, 2539, 1710, 2787, 3543, 1074, 4098, 3410, 1329, 3981, 1529, 2878, 1798, 3200, 4849, 3682, 1058, 2328, 2575, 4630, 4439, 1398, 1228, 2252, 1419, 3729, 1703, 1590, 4550, 3100, 2284, 1585, 2582, 2942, 3794, 1454, 1995, 3615, 4047, 1238, 1389, 4622, 2246, 3856, 2589, 1904, 3241, 2372, 4236, 2389, 4425, 2753, 1908, 4222, 1463, 2689, 1269, 2946, 2745, 3863, 3043, 4242, 4594, 3424, 3419, 4264, 4532, 3016, 4656, 2987, 2233, 3292, 1712, 3585, 4718, 3007, 3336, 1764, 4269, 3595, 2989, 3171, 4138, 4751, 2947, 3451, 2692, 1814, 1780, 3913, 3618, 2776, 4749, 2140, 4658, 4862, 2037, 3802, 1050, 2742, 2072, 4294, 1438, 4759, 2597, 4729, 1386, 1337, 3583, 2859, 2424, 2030, 1829, 1591, 2555, 2675, 2337, 3761, 4525, 3492, 1990, 2820, 2366, 1705, 3684, 4638, 2104, 1628, 2798, 1614, 3982, 2433, 1947, 3416, 2703, 1522, 4696, 1112, 1211, 3332, 3853, 2548, 2724, 1102, 1691, 2137, 4775, 2262, 3818, 3817, 1433, 3430, 1520, 3201, 1818, 1540, 3668, 1790, 2267, 4652, 3830, 1462, 1145, 1029, 3452, 1832, 2523, 2049, 1909, 1330, 2693, 2580, 4727, 3244, };
    INT arr_size = sizeof(arr) / sizeof(arr[0]);
    HANDLE hThread;
    DWORD IDThread;

    cout << "Given array is " << endl;
    printArray(arr, arr_size);

    hThread = CreateThread(NULL, 0, mergeSortThread, new ThreadArgs{ arr, 0, arr_size - 1 }, 0, &IDThread);

    if (hThread != NULL) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }

    cout << "Sorted array is " << endl;
    printArray(arr, arr_size);

    return 0;
}
