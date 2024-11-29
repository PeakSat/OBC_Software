#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpsabi" // Suppress: parameter passing for argument of type 'Time::DefaultCUC' {aka 'TimeStamp<4, 0, 1, 10>'} changed in GCC 7.1

#include "Helpers/Parameter.hpp"
#include "CAN/Driver.hpp"

namespace AcubeSATParameters {
    /**
     * ID enumeration of OBDH subsystem's specific parameters.
     */
    enum ParameterID : uint16_t {
        OBCUseRTT = 8,
        OBCUseUART = 9,
        OBCUseCAN = 10,

        /* OBDH Parameters */
        OBCPCBTemperature1 = 5000,
        OBCPCBTemperature2 = 5001,
        OBCMCUTemperature = 5002,
        OBCMCUInputVoltage = 5003,
        OBCMCUBootCounter = 5004,
        OBCFlashInt = 5005,
        OBCSRAMInt = 5006,
        OBCAvailableMRAM = 5007,
        OBCAvailableNAND = 5008,
        OBCSpacecraftTimeRef = 5009, ///< which subsystem holds the correct time
        OBCOnBoardTime = 5010,
        OBCOperationalMode = 5011,
        OBCMemoryPartition = 5012,
        OBCReconfigurationTimer = 5013, ///< timer responsible to reset the spacecraft to a known working state unless it has been
                                        ///< reset by a ground station pass.
        OBCLastFailedEvent = 5014,
        OBCMCUSystick = 5015,
        OBCCANBUSLoad1 = 5016, ///< the CAN bus load is based on the used capacity (bandwidth) divided by maximum capacity
        OBCCANBUSLoad2 = 5017,
        OBCCANBUSActive = 5018,
        OBCMCUFDIR = 5019,
        OBCMCURestartSafeModeThreshold = 5020,
        OBCNANDFLASHLCLThreshold = 5021,
        OBCMRAMLCLThreshold = 5022,
        OBCNANDFLASHON = 5023,
        OBCMRAMON = 5024,
        OBCNANDFLASHScrubbingFrequency = 5025,
        OBCRAMScrubbingFrequency = 5026,
        OBCProgramFlashScrubbingFrequency = 5027,

        /* SU */
        SUPCBTemperature1 = 4000,
        SUPCBTemperature2 = 4001,
        SUPCBTemperatureControlValves = 4002,
        SUPCBTemperatureFlowValves = 4003,
        SUPDMSTemperature = 4004,
        SUGrowthMediumTemperature = 4005,
        SULEDIntensity1 = 4006,
        SULEDIntensity2 = 4007,
        SULEDIntensity3 = 4008,
        SULEDIntensity4 = 4009,
        SUPressureLevelSensor1 = 4010,
        SUPressureLevelSensor2 = 4011,
        SUHumidityLevelSensor1 = 4012,
        SUHumidityLevelSensor2 = 4013,
        SUNumberOfPhotosTaken = 4014,
        SUPumpFlowRate1 = 4015,
        SUPumpFlowRate2 = 4016,
        SUControlChamberValve1 = 4017,
        SUControlChamberValve2 = 4018,
        SUControlChamberValve3 = 4019,
        SUControlSandwitchValve1 = 4020,
        SUControlSandwitchValve2 = 4021,
        SUControlSandwitchValve3 = 4022,
        SUFlowValveInlet1 = 4023,
        SUFlowValveInlet2 = 4024,
        SUFlowValveInlet3 = 4025,
        SUFlowValveOutlet1 = 4026,
        SUFlowValveOutlet2 = 4027,
        SUFlowValveOutlet3 = 4028,
        SUFlowValveTest1 = 4029,
        SUFlowValveTest2 = 4030,
        SUPhotoExposureTime = 4031,
        SUPhotoInterval = 4032,
        SUPhotoCroppedResolution = 4033,
        SUPhotoCompressionRate = 4034,
        SUExperimentNumber = 4035,
        SURADFET = 4036,
        SUCameraTemperature = 4037,
        SUMCUTemperature = 4038,
        SUMCUInputVoltage = 4039,
        SUMCUBootCounter = 4040,
        SUFlashIntUsedPercentage = 4041,
        SUFlashUsedPercentage = 4042,
        SUSRAMUsedPercentage = 4043,
        SUOnBoardTime = 4044,
        SUNANDCurrentlyUsedPartition = 4045,
        SULastFailedEvent = 4046,
        SUMCUSysTick = 4047,
        SUNANDFlashLCLThreshold = 4048,
        SUFlowPumpFlashLCLThreshold = 4049,
        SUControlPumpFlashLCLThreshold = 4050,
        SUCameraLCLThreshold = 4051,
        SUNANDFlashLCLOn = 4052,
        SUFlowPumpFlashOn = 4053,
        SUControlPumpFlashLCLOn = 4054,
        SUCameraON = 4055,
        SUNANDFlashScrubbingFrequency = 4056,
        SURAMScrubbingFrequency = 4057,
        SUProgramFlashScrubbingFrequency = 4058,
        SUTemperatureFromHumiditySensor1 = 4059,
        SUTemperatureFromHumiditySensor2 = 4060,


        /* ADCS */
        ADCSUseRTT = 11,
        ADCSUseUART = 12,
        ADCSUseCAN = 13,

        ADCSXBodyFrameRegardingOrbitFrame = 1000,
        ADCSYBodyFrameRegardingOrbitFrame = 1001,
        ADCSZBodyFrameRegardingOrbitFrame = 1002,
        ADCSAngularVelocityOnX = 1003,
        ADCSAngularVelocityOnY = 1004,
        ADCSAngularVelocityOnZ = 1005,
        ADCSBDotXAxis = 1006,
        ADCSBDotYAxis = 1007,
        ADCSBDotZAxis = 1008,
        ADCSQuaternionOrbitFrameToBodyScalar = 1009,
        ADCSQuaternionOrbitFrameToBodyVectorElem1 = 1010,
        ADCSQuaternionOrbitFrameToBodyVectorElem2 = 1011,
        ADCSQuaternionOrbitFrameToBodyVectorElem3 = 1012,

        ADCSMagnetometerRawX = 1013,
        ADCSMagnetometerRawY = 1014,
        ADCSMagnetometerRawZ = 1015,
        ADCSMagnetometerFrequency = 1016,
        ADCSMagnetometerCycleCountX = 1017,
        ADCSMagnetometerCycleCountY = 1018,
        ADCSMagnetometerCycleCountZ = 1019,
        ADCSMagnetometerSelfTest = 1020,

        ADCSMagnetometerISISCalibratedXAxis = 1021,
        ADCSMagnetometerISISCalibratedYAxis = 1022,
        ADCSMagnetometerISISCalibratedZAxis = 1023,
        ADCSMagnetometerISISFilteredXAxis = 1024,
        ADCSMagnetometerISISFilteredYAxis = 1025,
        ADCSMagnetometerISISFilteredZAxis = 1026,
        ADCSMagnetometerISISRawXAxis = 1027,
        ADCSMagnetometerISISRawYAxis = 1028,
        ADCSMagnetometerISISRawZAxis = 1029,
        ADCSFineSunSensorRawTopLeft = 1030,
        ADCSFineSunSensorRawTopRight = 1031,
        ADCSFineSunSensorRawBottomLeft = 1032,
        ADCSFineSunSensorRawBottomRight = 1033,
        ADCSFineSunSensorFilteredTopLeft = 1034,
        ADCSFineSunSensorFilteredTopRight = 1035,
        ADCSFineSunSensorFilteredBottomLeft = 1036,
        ADCSFineSunSensorFilteredBottomRight = 1037,
        ADCSFineSunSensorAngularAlphaAngle = 1038,
        ADCSFineSunSensorAngularBetaAngle = 1039,
        ADCSFineSunSensorCalculatedX = 1040,
        ADCSFineSunSensorCalculatedY = 1041,
        ADCSFineSunSensorCalculatedZ = 1042,

        ADCSGyroscopeRateX = 1043,
        ADCSGyroscopeRateY = 1044,
        ADCSGyroscopeRateZ = 1045,
        ADCSGyroXLOCSTRegister = 1046,
        ADCSGyroXHICSTRegister = 1047,
        ADCSGyroXQUADRegister = 1048,
        ADCSGyroXFAULTRegister = 1049,
        ADCSGyroYLOCSTRegister = 1050,
        ADCSGyroYHICSTRegister = 1051,
        ADCSGyroYQUADRegister = 1052,
        ADCSGyroYFAULTRegister = 1053,
        ADCSGyroZLOCSTRegister = 1054,
        ADCSGyroZHICSTRegister = 1055,
        ADCSGyroZQUADRegister = 1056,
        ADCSGyroZFAULTRegister = 1057,

        ADCSMagnetorquerOnXAxisHBridgePolarity = 1058,
        ADCSMagnetorquerOnYAxisHBridgePolarity = 1059,
        ADCSMagnetorquerOnZAxisHBridgePolarity = 1060,
        ADCSMagnetorquerOnXAxisOnOff = 1061,
        ADCSMagnetorquerOnYAxisOnOff = 1062,
        ADCSMagnetorquerOnZAxisOnOff = 1063,
        ADCSGainBdotXAxis = 1064,
        ADCSGainBdotYAxis = 1065,
        ADCSGainBdotZAxis = 1066,
        ADCSGainProportionalPDXAxis = 1067,
        ADCSGainProportionalPDYAxis = 1068,
        ADCSGainProportionalPDZAxis = 1069,
        ADCSGainDerivativePDXAxis = 1070,
        ADCSGainDerivativePDYAxis = 1071,
        ADCSGainDerivativePDZAxis = 1072,
        ADCSiMQTBoardCoilXTemperatureSensor = 1073,
        ADCSiMQTBoardCoilYTemperatureSensor = 1074,
        ADCSiMQTBoardCoilZTemperatureSensor = 1075,
        ADCSiMQTBoardExtraTemperatureSensor = 1076,

        ADCSGyroscopeXTemperature = 1077,
        ADCSGyroscopeYTemperature = 1078,
        ADCSGyroscopeZTemperature = 1079,
        ADCSBoardTemperature1 = 1080,
        ADCSBoardTemperature2 = 1081,

        ADCSMode = 1082,
        ADCSTLE = 1083,
        ADCSInitialKalmanQuaternionECIFrameToBodyScalar = 1084,
        ADCSInitialKalmanQuaternionECIFrameToBodyVectorElem1 = 1085,
        ADCSInitialKalmanQuaternionECIFrameToBodyVectorElem2 = 1086,
        ADCSInitialKalmanQuaternionECIFrameToBodyVectorElem3 = 1087,
        ADCSTLEOrbitalElementsArgumentPerigee = 1088,
        ADCSTLEOrbitalElementsRAAN = 1089,
        ADCSTLEOrbitalElementsInclination = 1090,
        ADCSTLEOrbitalElementsEccentricity = 1091,
        ADCSTLEOrbitalElementsMeanAnomaly = 1092,
        ADCSSGP4OrbitalElementsArgumentPerigee = 1093,
        ADCSSGP4OrbitalElementsRAAN = 1094,
        ADCSSGP4OrbitalElementsInclination = 1095,
        ADCSSGP4OrbitalElementsMeanAnomaly = 1096,
        ADCSTimeGST_JD = 1097,
        ADCSDeterminationInitialMatricesQ1 = 1098,
        ADCSDeterminationInitialMatricesQ2 = 1099,
        ADCSDeterminationInitialMatricesQ3 = 1100,
        ADCSDeterminationInitialMatricesQ4 = 1101,
        ADCSDeterminationInitialMatricesQ5 = 1102,
        ADCSDeterminationInitialMatricesQ6 = 1103,
        ADCSDeterminationInitialMatricesQ7 = 1104,
        ADCSDeterminationInitialMatricesQ8 = 1105,
        ADCSDeterminationInitialMatricesQ9 = 1106,
        ADCSDeterminationInitialMatricesQ10 = 1107,
        ADCSDeterminationInitialMatricesR1 = 1108,
        ADCSDeterminationInitialMatricesR2 = 1109,
        ADCSDeterminationInitialMatricesR3 = 1110,
        ADCSDeterminationInitialMatricesR4 = 1111,
        ADCSDeterminationInitialMatricesR5 = 1112,
        ADCSDeterminationInitialMatricesR6 = 1113,
        ADCSDeterminationInitialMatricesR7 = 1114,
        ADCSDeterminationInitialMatricesR8 = 1115,
        ADCSDeterminationInitialMatricesR9 = 1116,
        ADCSDeterminationInitialMatricesPInitial1 = 1117,
        ADCSDeterminationInitialMatricesPInitial2 = 1118,
        ADCSDeterminationInitialMatricesPInitial3 = 1119,
        ADCSDeterminationInitialMatricesPInitial4 = 1120,
        ADCSDeterminationInitialMatricesPInitial5 = 1121,
        ADCSDeterminationInitialMatricesPInitial6 = 1122,
        ADCSDeterminationInitialMatricesPInitial7 = 1123,
        ADCSDeterminationInitialMatricesPInitial8 = 1124,
        ADCSDeterminationInitialMatricesPInitial9 = 1125,
        ADCSDeterminationInitialMatricesPInitial10 = 1126,
        ADCSReactionWheelOn = 1127,
        ADCSReactionWheelAngularVelocity = 1128,
        ADCSMagnetorquerAxisAssignment = 1129,
        ADCSControlProfileSwitching = 1130,
        ADCSTransformationAxisXAssignment = 1131,
        ADCSTransformationAxisYAssignment = 1132,
        ADCSTransformationAxisZAssignment = 1133,
        ADCSMagnetometerAxisAssignment = 1134,
        ADCSSunSensorAxisAssignment = 1135,
        ADCSGyroscopeAxisAssignment = 1136,
        ADCSFrameAxisSignECIToECEF_X = 1137,
        ADCSFrameAxisSignECIToECEF_Y = 1138,
        ADCSFrameAxisSignECIToECEF_Z = 1139,
        ADCSFrameAxisSignECIToOrbit_X = 1140,
        ADCSFrameAxisSignECIToOrbit_Y = 1141,
        ADCSFrameAxisSignECIToOrbit_Z = 1142,
        ADCSFrameAxisSignNEDToECEF_X = 1143,
        ADCSFrameAxisSignNEDToECEF_Y = 1144,
        ADCSFrameAxisSignNEDToECEF_Z = 1145,

        ADCSMagnetometerSignX = 1146,
        ADCSMagnetometerSignY = 1147,
        ADCSMagnetometerSignZ = 1148,

        ADCSSunSensorXAxisSign = 1149,
        ADCSSunSensorYAxisSign = 1150,
        ADCSSunSensorZAxisSign = 1151,

        ADCSGyroSignX = 1152,
        ADCSGyroSignY = 1153,
        ADCSGyroSignZ = 1154,

        ADCSDetumblingNominalThresholds = 1155,
        ADCSNominalDetumblingThresholds = 1156,
        ADCSStandByDetumblingThresholds = 1157,
        ADCSDetumblingStandByThresholds = 1158,
        ADCSNominalStandByThresholds = 1159,
        ADCSMagnetorquerMagneticDipoleX = 1160,
        ADCSMagnetorquerMagneticDipoleY = 1161,
        ADCSMagnetorquerMagneticDipoleZ = 1162,
        ADCSMagnetorquerDutyCycle = 1163,
        ADCSMagnetorquerCurrentX = 1164,
        ADCSMagnetorquerCurrentY = 1165,
        ADCSMagnetorquerCurrentZ = 1166,
        ADCSMagnetorquerBDot = 1167,
        ADCSReactionWheelTorque = 1168,
        ADCSReactionWheelAngularAcceleration = 1169,
        ADCSDesiredQuaternionOrbitToBodyScalar = 1170,
        ADCSDesiredQuaternionVectorElem1 = 1171,
        ADCSDesiredQuaternionVectorElem2 = 1172,
        ADCSDesiredQuaternionVectorElem3 = 1173,
        ADCSAngularVelocityDesiredX = 1174,
        ADCSAngularVelocityDesiredY = 1175,
        ADCSAngularVelocityDesiredZ = 1176,

        ADCSGyroBiasX = 1177,
        ADCSGyroBiasY = 1178,
        ADCSGyroBiasZ = 1179,

        ADCSSatellitePositionLatitude = 1180,
        ADCSSatellitePositionLongitude = 1181,
        ADCSSatellitePositionHeight = 1182,
        ADCSSatellitePositionECIX = 1183,
        ADCSSatellitePositionECIY = 1184,
        ADCSSatellitePositionECIZ = 1185,
        ADCSSatelliteLinearVelocityECIX = 1186,
        ADCSSatelliteLinearVelocityECIY = 1187,
        ADCSSatelliteLinearVelocityECIZ = 1188,
        ADCSMagneticFieldNEDX = 1189,
        ADCSMagneticFieldNEDY = 1190,
        ADCSMagneticFieldNEDZ = 1191,
        ADCSMagneticFieldECIX = 1192,
        ADCSMagneticFieldECIY = 1193,
        ADCSMagneticFieldECIZ = 1194,
        ADCSSunPositionECIX = 1195,
        ADCSSunPositionECIY = 1196,
        ADCSSunPositionECIZ = 1197,
        ADCSEclipse = 1198,
        ADCSDesaturationPecentageParameter = 1199,
        ADCSISISMTQModes = 1200,
        ADCSMagnetorquerActuationMethod = 1201,
        ADCSDutyCycleDetermination = 1202,
        ADCSDutyCycleControl = 1203,
        ADCSDutyCycleOther = 1204,
        ADCSDesatInitialAngularVelocityParam = 1205,

        ADCSMCUTemperature = 1206,

        ADCSMCUInputVoltage = 1207,

        ADCSBootCounter = 1208,
        ADCSOnBoardTime = 1209,

        ADCSMemoryPartition = 1210,
        ADCSLastFailedEvent = 1211,

        ADCSSystick = 1212,

        ADCSRAMScrubbingFrequency = 1213,
        ADCSProgramFlashScrubbingFrequency = 1214,
        ADCSFLASHInt = 1215,
        ADCSSRAMInt = 1216,

        /* EPS Parameters */
        EPS_MODE = 3000,
        EPS_CONF = 3001,
        EPS_RESET_CAUSE = 3002,
        EPS_UPTIME = 3003,
        EPS_ERROR = 3004,
        EPS_RC_CNT_PWRON = 3005,
        EPS_RC_CNT_WDG = 3006,
        EPS_RC_CNT_CMD = 3007,
        EPS_RC_CNT_MCU = 3008,
        EPS_RC_CNT_EMLOPO = 3009,
        EPS_PREVCMD_ELAPSED = 3010,
        EPS_UNIX_TIME = 3011,
        EPS_UNIX_YEAR = 3012,
        EPS_UNIX_MONTH = 3013,
        EPS_UNIX_DAY = 3014,
        EPS_UNIX_HOUR = 3015,
        EPS_UNIX_MINUTE = 3016,
        EPS_UNIX_SECOND = 3017,
        EPS_STAT_CH_ON = 3018,
        EPS_STAT_CH_EXT_ON = 3019,
        EPS_STAT_CH_OCF = 3020,
        EPS_STAT_CH_EXT_OCF = 3021,
        EPS_OCF_CNT_CH00 = 3022,
        EPS_OCF_CNT_CH01 = 3023,
        EPS_OCF_CNT_CH02 = 3024,
        EPS_OCF_CNT_CH03 = 3025,
        EPS_OCF_CNT_CH04 = 3026,
        EPS_OCF_CNT_CH05 = 3027,
        EPS_OCF_CNT_CH06 = 3028,
        EPS_OCF_CNT_CH07 = 3029,
        EPS_OCF_CNT_CH08 = 3030,
        EPS_OCF_CNT_CH09 = 3031,
        EPS_OCF_CNT_CH10 = 3032,
        EPS_OCF_CNT_CH11 = 3033,
        EPS_OCF_CNT_CH12 = 3034,
        EPS_OCF_CNT_CH13 = 3035,
        EPS_OCF_CNT_CH14 = 3036,
        EPS_OCF_CNT_CH15 = 3037,
        EPS_OCF_CNT_CH16 = 3038,
        EPS_VOLT_BRDSUP_RAW = 3039,
        EPS_TEMP_RAW = 3040,
        EPS_VIP_DIST_INPUT_RAW = 3041,
        EPS_VIP_BATT_INPUT_RAW = 3042,
        EPS_VIP_VD0_RAW = 3043,
        EPS_VIP_VD1_RAW = 3044,
        EPS_VIP_VD2_RAW = 3045,
        EPS_VIP_VD3_RAW = 3046,
        EPS_VIP_VD4_RAW = 3047,
        EPS_VIP_VD5_RAW = 3048,
        EPS_VIP_VD6_RAW = 3049,
        EPS_VIP_VD7_RAW = 3050,
        EPS_VIP_VD8_RAW = 3051,
        EPS_VIP_VD9_RAW = 3052,
        EPS_VIP_VD10_RAW = 3053,
        EPS_VIP_VD11_RAW = 3054,
        EPS_VIP_V12_RAW = 3055,
        EPS_VIP_VD13_RAW = 3056,
        EPS_VIP_VD14_RAW = 3057,
        EPS_VIP_VD15_RAW = 3058,
        EPS_VOLT_BRDSUPraw = 3059,
        EPS_BAT_STAT = 3060,
        EPS_BAT_TEMP2 = 3061,
        EPS_VOLT_VD0_RAW = 3062,
        EPS_VOLT_VD1_RAW = 3063,
        EPS_VOLT_VD2_RAW = 3064,
        EPS_VIP_CH00_RAW = 3065,
        EPS_VIP_CH01_RAW = 3066,
        EPS_VIP_CH02_RAW = 3067,
        EPS_VIP_CH03_RAW = 3068,
        EPS_VIP_CH04_RAW = 3069,
        EPS_VIP_CH05_RAW = 3070,
        EPS_VIP_CH06_RAW = 3071,
        EPS_VIP_CH07_RAW = 3072,
        EPS_VIP_CH08_RAW = 3073,
        EPS_VIP_CH09_RAW = 3074,
        EPS_VIP_CH10_RAW = 3075,
        EPS_VIP_CH11_RAW = 3076,
        EPS_VIP_CH12_RAW = 3077,
        EPS_VIP_CH13_RAW = 3078,
        EPS_VIP_CH14_RAW = 3079,
        EPS_VIP_CH15_RAW = 3080,
        EPS_CC1_RAW = 3081,
        EPS_CC2_RAW = 3082,
        EPS_CC3_RAW = 3083,
        EPS_CC4_RAW = 3084,
        EPS_CC5_RAW = 3085,

        EPS_VOLT_VD0_ENG = 3086,
        EPS_VOLT_VD1_ENG = 3087,
        EPS_VOLT_VD2_ENG = 3088,
        EPS_VIP_CH00_ENG = 3089,
        EPS_VIP_CH01_ENG = 3090,
        EPS_VIP_CH02_ENG = 3091,
        EPS_VIP_CH03_ENG = 3092,
        EPS_VIP_CH04_ENG = 3093,
        EPS_VIP_CH05_ENG = 3094,
        EPS_VIP_CH06_ENG = 3095,
        EPS_VIP_CH07_ENG = 3096,
        EPS_VIP_CH08_ENG = 3097,

        //       TODO
        EPS_CC1_ENG = 3095,
        EPS_CC2_ENG = 3096,
        EPS_CC3_ENG = 3097,
        EPS_VOLT_VD0_RA = 3098,
        EPS_VOLT_VD1_RA = 3099,
        EPS_VOLT_VD2_RA = 3100,
        EPS_VIP_CH00_RA = 3101,
        EPS_VIP_CH01_RA = 3102,
        EPS_VIP_CH02_RA = 3103,
        EPS_VIP_CH03_RA = 3104,
        EPS_VIP_CH04_RA = 3105,
        EPS_VIP_CH05_RA = 3106,
        EPS_VIP_CH06_RA = 3106,
        EPS_VIP_CH07_RA = 3106,
        EPS_VIP_CH08_RA = 3106,
        EPS_CC1_RA = 3107,
        EPS_CC2_RA = 3108,
        EPS_CC3_RA = 3109,

        /* COMMS Parameters */
        COMMSUHFBandPATemperature = 2000,
        COMMSSBandPATemperature = 2001,
        COMMSPCBTemperature = 2002,
        COMMSAntennaDeploymentStatus = 2003,
        COMMSDataRateUHFTX = 2004,
        COMMSDataRateUHFRX = 2005,
        COMMSSymbolRateSBand = 2006,
        COMMSCWInterval = 2007,
        COMMSGMSKBeaconInterval = 2008,
        COMMSUHFBandTXPower = 2009,
        COMMSSBandTXPower = 2010,
        COMMSChannelNumberUHFBand = 2011,
        COMMSChannelNumberSBand = 2012,
        COMMSLNAGain = 2013,
        COMMSPAGainUHFBand = 2014,
        COMMSPAGainSBand = 2015,
        COMMSVGAGain = 2016,
        COMMSRSSI = 2017,
        COMMSUHFBandTXOnOff = 2018,
        COMMSUHFBandRXOnOff = 2019,
        COMMSSBandTXOnOff = 2020,
        COMMSPacketsRejectedCOMMS = 2021,
        COMMSInvalidHMAC = 2022,
        COMMSInvalidPacketStructure = 2023,
        COMMSInvalidSpacecraftID = 2024,
        COMMSFrameSequenceCounter = 2025,
        COMMSPCBTemperature1 = 2026,
        COMMSPCBTemperature2 = 2027,
        COMMSMCUTemperature = 2028,
        COMMSMCUInputVoltage = 2029,
        COMMSMCUBootCounter = 2030,
        COMMSOnBoardTime = 2031,
        COMMSNANDCurrentlyUsedMemoryPartition = 2032,
        COMMSLastFailedEvent = 2033,
        COMMSMCUSystick = 2034,
        COMMSFlashInt = 2035,
        COMMSSRAMInt = 2036
    };

    /******************* OBDH ENUMS *******************/
    enum SpacecraftTimeRef : uint8_t {
        Spacecraft = 0,
        GroundStation = 1
    };

    enum OperationalMode : uint8_t {
        CommissioningMode = 0,
        NominalMode = 1,
        ScienceMode = 2,
        SafeMode = 3
    };

    enum MemoryPartition : uint8_t {
        First = 0,
        Second = 1
    };

    enum CANBUSActive : uint8_t {
        Main = 0,
        Reductant = 1
    };

    enum MCUFDIR : uint8_t {
        OBC = 0,
        ADCS = 1
    };

    /******************* ADCS ENUMS *******************/
    enum ADCSModeOfOperation : uint8_t {
        StandByModeADCS = 0,
        DetumbingModeADCS = 1,
        NominalModeADCS = 2
    };

    enum AxisAssignment : uint8_t {
        Xaxis = 0,
        Yaxis = 1,
        Zaxis = 2
    };

    enum MangetorquerDutyCycle : uint8_t {
        VeryHigh = 0,
        High = 1,
        Medium = 2,
        Low = 3
    };

    enum MagnetorquerModes : uint8_t {
        Idle = 0,
        SelfTest = 1,
        Detumbling = 2
    };

    enum MagnetorquerActuationMethod : uint8_t {
        Current = 0,
        Dipole = 1,
        PWM = 2
    };

    /******************* SU ENUMS *******************/
    enum CroppingResolution : uint64_t {
        Default = 0
    };

    enum CompressionRate : uint32_t {
        None = 0
    };

    enum ExprerimentNumber : uint8_t {
        FirstExperiment = 0,
        SecondExperiment = 1,
        ThirdExperiment = 2
    };

    /******************* COMMS ENUMS *******************/
    enum AntennaDeploymentStatus : uint8_t {
        Closed = 0,
        OneDoorOpen = 1,
        TwoDoorOpen = 2,
        ThreeDoorOpen = 3,
        FullyDeployed = 4
    };

    enum SampleRateUHFTX : uint8_t {
        Rate = 0
    };

    enum AntennaGains : uint8_t {
        Gain = 0
    };

    /******************* OBDH PARAMETERS *******************/
    inline Parameter<bool> obcUseRTT(true);
    inline Parameter<bool> obcUseUART(true);
    inline Parameter<bool> obcUseCAN(false);

    inline Parameter<float> obcPCBTemperature1(0);
    inline Parameter<float> obcPCBTemperature2(0);
    inline Parameter<float> obcMCUTemperature(0);
    inline Parameter<float> obcMCUInputVoltage(0);

    inline Parameter<uint16_t> obcMCUBootCounter(0);
    inline Parameter<uint32_t> obcFlashInt(0);
    inline Parameter<uint32_t> obcSRAMInt(0);
    inline Parameter<uint32_t> obcAvailableMRAM(0);
    inline Parameter<uint32_t> obcAvailableNAND(0);

    inline Parameter<Time::DefaultCUC> obcOnBoardTime(Time::DefaultCUC(0));

    inline Parameter<SpacecraftTimeRef> obcSpacecraftTimeRef(Spacecraft); // enum
    inline Parameter<OperationalMode> obcOperationalMode(NominalMode);    // enum
    inline Parameter<MemoryPartition> obcMemoryPartition(First);          // enum

    inline Parameter<uint32_t> obcReconfigurationTimer(0);
    inline Parameter<uint16_t> obcLastFailedEvent(0);
    inline Parameter<uint32_t> obcMCUSystick(0);

    inline Parameter<float> obcCANBUSLoad1(0);
    inline Parameter<float> obcCANBUSLoad2(0);

    inline Parameter<CAN::Driver::ActiveBus> obcCANBUSActive(CAN::Driver::Main); // enum
    inline Parameter<MCUFDIR> obcMCUFDIR(OBC);                                   // enum

    inline Parameter<uint8_t> obcMCURestartSafeModeThreshold(0);
    inline Parameter<float> obcNANDFLASHLCLThreshold(0);
    inline Parameter<float> obcMRAMLCLThreshold(0);
    inline Parameter<float> obcNANDFLASHON(0);
    inline Parameter<float> obcNANDFlashScrubbingFrequency(0);
    inline Parameter<float> obcMRAMON(0);
    inline Parameter<float> obcRAMScrubbingFrequency(0);
    inline Parameter<float> obcProgramFlashScrubbingFrequency(0);

    /******************* SU PARAMETERS *******************/
    // MCU
    inline Parameter<uint32_t> suMCUBootCounter(0);
    inline Parameter<uint32_t> suMCUSysTick(0);

    inline Parameter<float> suMCUTemperature(0);
    inline Parameter<float> suMCUInputVoltage(0);

    // Memory
    inline Parameter<uint32_t> suFlashIntUsedPercentage(0);
    inline Parameter<uint32_t> suFlashUsedPercentage(0);
    inline Parameter<uint32_t> suSRAMUsedPercentage(0);

    inline Parameter<MemoryPartition> suNANDCurrentlyUsedPartition(First); // enum
    inline Parameter<float> suNANDFlashLclOn(0);
    inline Parameter<float> suNANDFlashLclThreshold(0);

    inline Parameter<float> suNANDFlashScrubbingFrequency(0);
    inline Parameter<float> suRAMScrubbingFrequency(0);
    inline Parameter<float> suProgramFlashScrubbingFrequency(0);

    // Components
    inline Parameter<float> suControlPumpFlashLclOn(0);
    inline Parameter<float> suControlPumpFlashLclThreshold(0);

    inline Parameter<float> suFlowPumpFlashOn(0);
    inline Parameter<float> suFlowPumpFlashLclThreshold(0);

    inline Parameter<float> suCameraOn(0);
    inline Parameter<float> suCameraLCLThreshold(0);

    // Temperatures
    inline Parameter<float> suPCBTemperature1(0);
    inline Parameter<float> suPCBTemperature2(0);

    inline Parameter<float> suPCBTemperatureControlValves(0);
    inline Parameter<float> suPCBTemperatureFlowValves(0);

    inline Parameter<float> suCameraTemperature(0);
    inline Parameter<float> suPDMSTemperature(0);
    inline Parameter<float> suGrowthMediumTemperature(0);

    // Sensors
    inline Parameter<float> suPressureLevelSensor1(0);
    inline Parameter<float> suPressureLevelSensor2(0);

    inline Parameter<float> suHumidityLevelSensor1(0);
    inline Parameter<float> suHumidityLevelSensor2(0);

    inline Parameter<double> suRadfet(0);

    // Components
    inline Parameter<float> suPumpFlowRate1(0);
    inline Parameter<float> suPumpFlowRate2(0);

    inline Parameter<bool> suCcontrolChamberValve1(0);
    inline Parameter<bool> suControlChamberValve2(0);
    inline Parameter<bool> suControlChamberValve3(0);

    inline Parameter<bool> suControlSandwitchValve1(0);
    inline Parameter<bool> suControlSandwitchValve2(0);
    inline Parameter<bool> suControlSandwitchValve3(0);

    inline Parameter<bool> suFlowValveInlet1(0);
    inline Parameter<bool> suFlowValveInlet2(0);
    inline Parameter<bool> suFlowValveInlet3(0);

    inline Parameter<bool> suFlowValveOutlet1(0);
    inline Parameter<bool> suFlowValveOutlet2(0);
    inline Parameter<bool> suFlowValveOutlet3(0);

    inline Parameter<bool> suFlowValveTest1(0);
    inline Parameter<bool> suFlowValveTest2(0);

    inline Parameter<float> suLEDIntensity1(0);
    inline Parameter<float> suLEDIntensity2(0);
    inline Parameter<float> suLEDIntensity3(0);
    inline Parameter<float> suLEDIntensity4(0);

    // Photos
    inline Parameter<uint16_t> suNumberOfPhotosTaken(0);
    inline Parameter<float> suPhotoExposureTime(0);
    inline Parameter<uint16_t> suPhotoInterval(0);
    inline Parameter<CroppingResolution> suPhotoCroppedResolution(Default); // enum
    inline Parameter<CompressionRate> suPhotoCompressionRate(None);         // enum

    // Misc
    inline Parameter<ExprerimentNumber> suExperimentNumber(FirstExperiment); // enum
    inline Parameter<Time::DefaultCUC> suOnBoardTime(Time::DefaultCUC(0));
    inline Parameter<uint16_t> suLastFailedEvent(0);

    inline Parameter<float> suTemperatureFromHumiditySensor1(0);
    inline Parameter<float> suTemperatureFromHumiditySensor2(0);

    /******************* ADCS PARAMETERS *******************/
    inline Parameter<bool> adcsUseRTT(false);
    inline Parameter<bool> adcsUseUART(true);
    inline Parameter<bool> adcsUseCAN(false);

    inline Parameter<float> adcsXBodyFrameRegardingOrbitFrame(0);
    inline Parameter<float> adcsYBodyFrameRegardingOrbitFrame(0);
    inline Parameter<float> adcsZBodyFrameRegardingOrbitFrame(0);
    inline Parameter<float> adcsAngularVelocityOnX(0);
    inline Parameter<float> adcsAngularVelocityOnY(0);
    inline Parameter<float> adcsAngularVelocityOnZ(0);
    inline Parameter<float> adcsBDotXAxis(0);
    inline Parameter<float> adcsBDotYAxis(0);
    inline Parameter<float> adcsBDotZAxis(0);
    inline Parameter<float> adcsQuaternionOrbitFrameToBodyScalar(0);
    inline Parameter<float> adcsQuaternionOrbitFrameToBodyVectorElem1(0);
    inline Parameter<float> adcsQuaternionOrbitFrameToBodyVectorElem2(0);
    inline Parameter<float> adcsQuaternionOrbitFrameToBodyVectorElem3(0);

    inline Parameter<double> adcsMagnetometerRawX(0);
    inline Parameter<double> adcsMagnetometerRawY(0);
    inline Parameter<double> adcsMagnetometerRawZ(0);
    inline Parameter<uint8_t> adcsMagnetometerFrequency(0);
    inline Parameter<int8_t> adcsMagnetometerCycleCountX(0);
    inline Parameter<int8_t> adcsMagnetometerCycleCountY(0);
    inline Parameter<int8_t> adcsMagnetometerCycleCountZ(0);
    inline Parameter<bool> adcsMagnetometerSelfTest(0);

    inline Parameter<int32_t> adcsMagnetometerISISCalibratedXAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISCalibratedYAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISCalibratedZAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISFilteredXAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISFilteredYAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISFilteredZAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISRawXAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISRawYAxis(0);
    inline Parameter<int32_t> adcsMagnetometerISISRawZAxis(0);

    inline Parameter<float> adcsFineSunSensorRawTopLeft(0);
    inline Parameter<float> adcsFineSunSensorRawTopRight(0);
    inline Parameter<float> adcsFineSunSensorRawBottomLeft(0);
    inline Parameter<float> adcsFineSunSensorRawBottomRight(0);
    inline Parameter<float> adcsFineSunSensorFilteredTopLeft(0);
    inline Parameter<float> adcsFineSunSensorFilteredTopRight(0);
    inline Parameter<float> adcsFineSunSensorFilteredBottomLeft(0);
    inline Parameter<float> adcsFineSunSensorFilteredBottomRight(0);
    inline Parameter<float> adcsFineSunSensorAngularAlphaAngle(0);
    inline Parameter<float> adcsFineSunSensorAngularBetaAngle(0);
    inline Parameter<float> adcsFineSunSensorCalculatedX(0);
    inline Parameter<float> adcsFineSunSensorCalculatedY(0);
    inline Parameter<float> adcsFineSunSensorCalculatedZ(0);

    inline Parameter<float> adcsGyroscopeRateX(0);
    inline Parameter<float> adcsGyroscopeRateY(0);
    inline Parameter<float> adcsGyroscopeRateZ(0);

    inline Parameter<uint16_t> adcsGyroXLOCSTRegister(0);
    inline Parameter<uint16_t> adcsGyroXHICSTRegister(0);
    inline Parameter<uint16_t> adcsGyroXQUADRegister(0);
    inline Parameter<uint16_t> adcsGyroXFAULTRegister(0);

    inline Parameter<uint16_t> adcsGyroYLOCSTRegister(0);
    inline Parameter<uint16_t> adcsGyroYHICSTRegister(0);
    inline Parameter<uint16_t> adcsGyroYQUADRegister(0);
    inline Parameter<uint16_t> adcsGyroYFAULTRegister(0);

    inline Parameter<uint16_t> adcsGyroZLOCSTRegister(0);
    inline Parameter<uint16_t> adcsGyroZHICSTRegister(0);
    inline Parameter<uint16_t> adcsGyroZQUADRegister(0);
    inline Parameter<uint16_t> adcsGyroZFAULTRegister(0);

    inline Parameter<bool> adcsMagnetorquerOnXAxisHBridgePolarity(0);
    inline Parameter<bool> adcsMagnetorquerOnYAxisHBridgePolarity(0);
    inline Parameter<bool> adcsMagnetorquerOnZAxisHBridgePolarity(0);
    inline Parameter<bool> adcsMagnetorquerOnXAxisOnOff(0);
    inline Parameter<bool> adcsMagnetorquerOnYAxisOnOff(0);
    inline Parameter<bool> adcsMagnetorquerOnZAxisOnOff(0);

    inline Parameter<float> adcsGainBdotXAxis(0);
    inline Parameter<float> adcsGainBdotYAxis(0);
    inline Parameter<float> adcsGainBdotZAxis(0);
    inline Parameter<float> adcsGainProportionalPDXAxis(0);
    inline Parameter<float> adcsGainProportionalPDYAxis(0);
    inline Parameter<float> adcsGainProportionalPDZAxis(0);
    inline Parameter<float> adcsGainDerivativePDXAxis(0);
    inline Parameter<float> adcsGainDerivativePDYAxis(0);
    inline Parameter<float> adcsGainDerivativePDZAxis(0);

    inline Parameter<int16_t> adcsiMQTBoardCoilXTemperatureSensor(0);
    inline Parameter<int16_t> adcsiMQTBoardCoilYTemperatureSensor(0);
    inline Parameter<int16_t> adcsiMQTBoardCoilZTemperatureSensor(0);
    inline Parameter<int16_t> adcsiMQTBoardExtraTemperatureSensor(0);

    inline Parameter<float> adcsGyroscopeXTemperature(0);
    inline Parameter<float> adcsGyroscopeYTemperature(0);
    inline Parameter<float> adcsGyroscopeZTemperature(0);

    inline Parameter<float> adcsBoardTemperature1(0);
    inline Parameter<float> adcsBoardTemperature2(0);

    inline Parameter<ADCSModeOfOperation> adcsMode(StandByModeADCS); // enum
    inline Parameter<uint8_t> adcsTLE(0);

    inline Parameter<float> adcsInitialKalmanQuaternionECIFrameToBodyScalar(0);
    inline Parameter<float> adcsInitialKalmanQuaternionECIFrameToBodyVectorElem1(0);
    inline Parameter<float> adcsInitialKalmanQuaternionECIFrameToBodyVectorElem2(0);
    inline Parameter<float> adcsInitialKalmanQuaternionECIFrameToBodyVectorElem3(0);
    inline Parameter<float> adcsTLEOrbitalElementsArgumentPerigee(0);
    inline Parameter<float> adcsTLEOrbitalElementsRAAN(0);
    inline Parameter<float> adcsTLEOrbitalElementsInclination(0);
    inline Parameter<float> adcsTLEOrbitalElementsEccentricity(0);
    inline Parameter<float> adcsTLEOrbitalElementsMeanAnomaly(0);

    inline Parameter<double> adcsSGP4OrbitalElementsArgumentPerigee(0);
    inline Parameter<double> adcsSGP4OrbitalElementsRAAN(0);
    inline Parameter<double> adcsSGP4OrbitalElementsInclination(0);
    inline Parameter<double> adcsSGP4OrbitalElementsMeanAnomaly(0);
    inline Parameter<double> adcsTimeGST_JD(0); ///< Greenwich Sidereal Time

    inline Parameter<float> adcsDeterminationInitialMatricesQ1(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ2(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ3(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ4(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ5(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ6(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ7(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ8(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ9(0);
    inline Parameter<float> adcsDeterminationInitialMatricesQ10(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR1(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR2(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR3(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR4(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR5(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR6(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR7(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR8(0);
    inline Parameter<float> adcsDeterminationInitialMatricesR9(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial1(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial2(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial3(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial4(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial5(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial6(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial7(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial8(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial9(0);
    inline Parameter<float> adcsDeterminationInitialMatricesPInitial10(0);

    inline Parameter<bool> adcsReactionWheelOn(0);
    inline Parameter<float> adcsReactionWheelAngularVelocity(0);
    inline Parameter<bool> adcsControlProfileSwitching(0);

    inline Parameter<AxisAssignment> adcsMagnetorquerAxisAssignment(Xaxis);    // enum
    inline Parameter<AxisAssignment> adcsTransformationAxisXAssignment(Xaxis); // enum
    inline Parameter<AxisAssignment> adcsTransformationAxisYAssignment(Xaxis); // enum
    inline Parameter<AxisAssignment> adcsTransformationAxisZAssignment(Xaxis); // enum
    inline Parameter<AxisAssignment> adcsMagnetometerAxisAssignment(Xaxis);    // enum
    inline Parameter<AxisAssignment> adcsSunSensorAxisAssignment(Xaxis);       // enum
    inline Parameter<AxisAssignment> adcsGyroscopeAxisAssignment(Xaxis);       // enum

    inline Parameter<bool> adcsFrameAxisSignECIToECEF_X(0);
    inline Parameter<bool> adcsFrameAxisSignECIToECEF_Y(0);
    inline Parameter<bool> adcsFrameAxisSignECIToECEF_Z(0);
    inline Parameter<bool> adcsFrameAxisSignECIToOrbit_X(0);
    inline Parameter<bool> adcsFrameAxisSignECIToOrbit_Y(0);
    inline Parameter<bool> adcsFrameAxisSignECIToOrbit_Z(0);
    inline Parameter<bool> adcsFrameAxisSignNEDToECEF_X(0);
    inline Parameter<bool> adcsFrameAxisSignNEDToECEF_Y(0);
    inline Parameter<bool> adcsFrameAxisSignNEDToECEF_Z(0);

    inline Parameter<bool> adcsMagnetometerSignX(0);
    inline Parameter<bool> adcsMagnetometerSignY(0);
    inline Parameter<bool> adcsMagnetometerSignZ(0);

    inline Parameter<bool> adcsSunSensorXAxisSign(0);
    inline Parameter<bool> adcsSunSensorYAxisSign(0);
    inline Parameter<bool> adcsSunSensorZAxisSign(0);

    inline Parameter<bool> adcsGyroSignX(0);
    inline Parameter<bool> adcsGyroSignY(0);
    inline Parameter<bool> adcsGyroSignZ(0);

    inline Parameter<float> adcsDetumblingNominalThresholds(0);
    inline Parameter<float> adcsNominalDetumblingThresholds(0);
    inline Parameter<float> adcsStandByDetumblingThresholds(0);
    inline Parameter<float> adcsDetumblingStandByThresholds(0);
    inline Parameter<float> adcsNominalStandByThresholds(0);
    inline Parameter<float> adcsMagnetorquerMagneticDipoleX(0);
    inline Parameter<float> adcsMagnetorquerMagneticDipoleY(0);
    inline Parameter<float> adcsMagnetorquerMagneticDipoleZ(0);

    inline Parameter<MangetorquerDutyCycle> adcsMagnetorquerDutyCycle(Low); // enum

    inline Parameter<int16_t> adcsMagnetorquerCurrentX(0);
    inline Parameter<int16_t> adcsMagnetorquerCurrentY(0);
    inline Parameter<int16_t> adcsMagnetorquerCurrentZ(0);

    inline Parameter<float> adcsMagnetorquerBDot(0);
    inline Parameter<float> adcsReactionWheelTorque(0);
    inline Parameter<float> adcsReactionWheelAngularAcceleration(0);
    inline Parameter<float> adcsDesiredQuaternionOrbitToBodyScalar(0);
    inline Parameter<float> adcsDesiredQuaternionVectorElem1(0);
    inline Parameter<float> adcsDesiredQuaternionVectorElem2(0);
    inline Parameter<float> adcsDesiredQuaternionVectorElem3(0);
    inline Parameter<float> adcsAngularVelocityDesiredX(0);
    inline Parameter<float> adcsAngularVelocityDesiredY(0);
    inline Parameter<float> adcsAngularVelocityDesiredZ(0);

    inline Parameter<float> adcsGyroBiasX(0);
    inline Parameter<float> adcsGyroBiasY(0);
    inline Parameter<float> adcsGyroBiasZ(0);

    inline Parameter<double> adcsSatellitePositionLatitude(0);
    inline Parameter<double> adcsSatellitePositionLongitude(0);
    inline Parameter<double> adcsSatellitePositionHeight(0);

    inline Parameter<float> adcsSatellitePositionECIX(0);
    inline Parameter<float> adcsSatellitePositionECIY(0);
    inline Parameter<float> adcsSatellitePositionECIZ(0);
    inline Parameter<float> adcsSatelliteLinearVelocityECIX(0);
    inline Parameter<float> adcsSatelliteLinearVelocityECIY(0);
    inline Parameter<float> adcsSatelliteLinearVelocityECIZ(0);
    inline Parameter<float> adcsMagneticFieldNEDX(0);
    inline Parameter<float> adcsMagneticFieldNEDY(0);
    inline Parameter<float> adcsMagneticFieldNEDZ(0);
    inline Parameter<float> adcsMagneticFieldECIX(0);
    inline Parameter<float> adcsMagneticFieldECIY(0);
    inline Parameter<float> adcsMagneticFieldECIZ(0);
    inline Parameter<float> adcsSunPositionECIX(0);
    inline Parameter<float> adcsSunPositionECIY(0);
    inline Parameter<float> adcsSunPositionECIZ(0);

    inline Parameter<bool> adcsEclipse(0);

    inline Parameter<float> adcsDesaturationPecentageParameter(0);

    inline Parameter<MagnetorquerModes> adcsISISMTQModes(Idle);                             // enum
    inline Parameter<MagnetorquerActuationMethod> adcsMagnetorquerActuationMethod(Current); // enum

    inline Parameter<float> adcsDutyCycleDetermination(0);
    inline Parameter<float> adcsDutyCycleControl(0);
    inline Parameter<float> adcsDutyCycleOther(0);
    inline Parameter<float> adcsDesatInitialAngularVelocityParam(0);

    inline Parameter<float> adcsMCUTemperature(0);
    inline Parameter<float> adcsMCUInputVoltage(0);
    inline Parameter<uint16_t> adcsBootCounter(0);
    inline Parameter<Time::DefaultCUC> adcsOnBoardTime(Time::DefaultCUC(0));
    inline Parameter<MemoryPartition> adcsMemoryPartition(First); // enum
    inline Parameter<int32_t> adcsLastFailedEvent(0);
    inline Parameter<uint64_t> adcsSystick(0);
    inline Parameter<float> adcsRAMScrubbingFrequency(0);
    inline Parameter<float> adcsProgramFlashScrubbingFrequency(0);
    inline Parameter<uint32_t> adcsSRAMInt(0);
    inline Parameter<uint32_t> adcsFlashInt(0);

    /****************** EPS PARAMETERS *******************/
    inline Parameter<uint8_t> epsMODE(0);
    inline Parameter<uint8_t> epsCONF(0);
    inline Parameter<uint8_t> epsRESET_CAUSE(0);
    inline Parameter<uint32_t> epsUPTIME(0);
    inline Parameter<uint16_t > epsERROR(0);
    inline Parameter<uint16_t > epsRC_CNT_PWRON(0);
    inline Parameter<uint16_t> epsRC_CNT_WDG(0);
    inline Parameter<uint16_t> epsRC_CNT_CMD(0);
    inline Parameter<uint16_t> epsRC_CNT_MCU(0);
    inline Parameter<uint16_t> epsRC_CNT_EMLOPO(0);
    inline Parameter<uint16_t> epsPREVCMD_ELAPSED(0);
    inline Parameter<uint32_t > epsUNIX_TIME(0);
    inline Parameter<uint8_t> epsUNIX_YEAR(0);

    inline Parameter<uint8_t> epsUNIX_MONTH(0);
    inline Parameter<uint8_t > epsUNIX_DAY(0);
    inline Parameter<uint8_t> epsUNIX_HOUR(0);
    inline Parameter<uint8_t> epsUNIX_MINUTE(0);
    inline Parameter<uint8_t > epsUNIX_SECOND(0);
    inline Parameter<uint16_t> epsSTAT_CH_ON(0);
    inline Parameter<uint16_t> epsSTAT_CH_EXT_ON(0);
    inline Parameter<uint16_t> epsSTAT_CH_OCF(0);
    inline Parameter<uint16_t> epsSTAT_CH_EXT_OCF(0);
    inline Parameter<uint16_t> epsOCF_CNT_CH00(0);
    inline Parameter<uint16_t> epsOCF_CNT_CH01(0);
    inline Parameter<uint16_t> epsOCF_CNT_CH02(0);
    inline Parameter<uint16_t> epsOCF_CNT_CH03(0);

    inline Parameter<uint16_t> epsOCF_CNT_CH04(0);
    inline Parameter<uint16_t> epsOCF_CNT_CH05(0);
    inline Parameter<uint16_t> epsOCF_CNT_CH06(0);
    inline Parameter<uint16_t> epsOCF_CNT_CH07(0);
    inline Parameter<uint16_t> epsOCF_CNT_CH08(0);
    inline Parameter<uint16_t> epsOCF_CNT_CH09(0);
    inline Parameter<uint16_t> epsOCF_CNT_CH10(0);
    inline Parameter<uint16_t> epsOCF_CNT_CH11(0);
    inline Parameter<uint16_t> epsOCF_CNT_CH12(0);
    inline Parameter<uint16_t> epsOCF_CNT_CH13(0);
    inline Parameter<uint16_t> epsOCF_CNT_CH14(0);
    inline Parameter<uint16_t> epsOCF_CNT_CH15(0);

    inline Parameter<uint16_t> epsOCF_CNT_CH16(0);
    inline Parameter<uint16_t> epsVOLT_BRDSUP_RAW(0);
    inline Parameter<uint16_t> epsTEMP_RAW(0);
    inline Parameter<int16_t> epsVIP_DIST_INPUT_RAW(0);
    inline Parameter<int16_t> epsVIP_BATT_INPUT_RAW(0);

    inline Parameter<int16_t> epsVIP_VD0_RAW(0);
    inline Parameter<int16_t> epsVIP_VD1_RAW(0);
    inline Parameter<int16_t> epsVIP_VD2_RAW(0);
    inline Parameter<int16_t> epsVIP_VD3_RAW(0);
    inline Parameter<int16_t> epsVIP_VD4_RAW(0);

    inline Parameter<int16_t> epsVIP_VD5_RAW(0);

    inline Parameter<int16_t> epsVIP_VD6_RAW(0);
    inline Parameter<int16_t> epsVIP_VD7_RAW(0);
    inline Parameter<int16_t> epsVIP_VD8_RAW(0);
    inline Parameter<int16_t> epsVIP_VD9_RAW(0);
    inline Parameter<int16_t> epsVIP_VD10_RAW(0);

    inline Parameter<int16_t> epsVIP_VD11_RAW(0);
    inline Parameter<int16_t> epsVIP_V12_RAW(0);

    inline Parameter<int16_t> epsVIP_VD13_RAW(0);
    inline Parameter<int16_t> epsVIP_VD14_RAW(0);
    inline Parameter<int16_t> epsVIP_VD15_RAW(0);

    inline Parameter<int16_t> epsVOLT_BRDSUPraw(0);
    inline Parameter<int16_t> epsBAT_STAT(0);
    inline Parameter<int16_t> epsBAT_TEMP1(0);
    inline Parameter<int16_t> epsBAT_TEMP2(0);
    inline Parameter<uint16_t> epsVOLT_VD0_RAW(0);
    inline Parameter<uint16_t> epsVOLT_VD1_RAW(0);
    inline Parameter<uint16_t> epsVOLT_VD2_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH00_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH01_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH02_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH03_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH04_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH05_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH06_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH07_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH08_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH09_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH10_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH11_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH12_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH13_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH14_RAW(0);
    inline Parameter<uint16_t> epsVIP_CH15_RAW(0);
    inline Parameter<uint16_t> epsCC1_RAW(0);
    inline Parameter<uint16_t> epsCC2_RAW(0);
    inline Parameter<uint16_t> epsCC3_RAW(0);
    inline Parameter<uint16_t> epsCC4_RAW(0);
    inline Parameter<uint16_t> epsCC5_RAW(0);

    inline Parameter<int16_t> epsVOLT_VD0_ENG(0);
    inline Parameter<int16_t> epsVOLT_VD1_ENG(0);
    inline Parameter<int16_t> epsVOLT_VD2_ENG(0);
    inline Parameter<int16_t> eps_VIP_CH00_ENG(0);
    inline Parameter<int16_t> eps_VIP_CH01_ENG(0);
    inline Parameter<int16_t> eps_VIP_CH02_ENG(0);
    inline Parameter<int16_t> eps_VIP_CH03_ENG(0);
    inline Parameter<int16_t> eps_VIP_CH04_ENG(0);
    inline Parameter<int16_t> eps_VIP_CH05_ENG(0);
    inline Parameter<int16_t> eps_VIP_CH06_ENG(0);
    inline Parameter<int16_t> eps_VIP_CH07_ENG(0);
    inline Parameter<int16_t> eps_VIP_CH08_ENG(0);
    inline Parameter<int16_t> epsCC1_ENG(0);
    inline Parameter<int16_t> epsCC2_ENG(0);
    inline Parameter<int16_t> epsCC3_ENG(0);

    inline Parameter<int16_t> epsVOLT_VD0_RA(0);
    inline Parameter<int16_t> epsVOLT_VD1_RA(0);
    inline Parameter<int16_t> epsVOLT_VD2_RA(0);
    inline Parameter<int16_t> eps_VIP_CH00_RA(0);
    inline Parameter<int16_t> eps_VIP_CH01_RA(0);
    inline Parameter<int16_t> eps_VIP_CH02_RA(0);
    inline Parameter<int16_t> eps_VIP_CH03_RA(0);
    inline Parameter<int16_t> eps_VIP_CH04_RA(0);
    inline Parameter<int16_t> eps_VIP_CH05_RA(0);
    inline Parameter<int16_t> eps_VIP_CH06_RA(0);
    inline Parameter<int16_t> eps_VIP_CH07_RA(0);
    inline Parameter<int16_t> eps_VIP_CH08_RA(0);
    inline Parameter<int16_t> epsCC1_RA(0);
    inline Parameter<int16_t> epsCC2_RA(0);
    inline Parameter<int16_t> epsCC3_RA(0);

    /****************** COMMS PARAMETERS *******************/
    inline Parameter<float> commsUHFBandPATemperature(0);
    inline Parameter<float> commsSBandPATemperature(0);
    inline Parameter<float> commsPCBTemperature(0);

    inline Parameter<AntennaDeploymentStatus> commsAntennaDeploymentStatus(Closed); // enum

    inline Parameter<SampleRateUHFTX> commsDataRateUHFTX(Rate); // enum
    inline Parameter<uint32_t> commsDataRateUHFRX(0);
    inline Parameter<uint32_t> commsSymbolRateSBand(0);
    inline Parameter<uint16_t> commsCWInterval(0);
    inline Parameter<uint16_t> commsGMSKBeaconInterval(0);
    inline Parameter<uint32_t> commsUHFBandTXPower(0);
    inline Parameter<uint32_t> commsSBandTXPower(0);
    inline Parameter<uint32_t> commsChannelNumberUHFBand(0);
    inline Parameter<uint32_t> commsChannelNumberSBand(0);

    inline Parameter<AntennaGains> commsLNAGain(Gain);       // enum
    inline Parameter<AntennaGains> commsPAGainUHFBand(Gain); // enum
    inline Parameter<AntennaGains> commsPAGainSBand(Gain);   // enum

    inline Parameter<uint8_t> commsVGAGain(0);
    inline Parameter<float> commsRSSI(0);

    inline Parameter<bool> commsUHFBandTXOnOff(0);
    inline Parameter<bool> commsUHFBandRXOnOff(0);
    inline Parameter<bool> commsSBandTXOnOff(0);

    inline Parameter<uint16_t> commsPacketsRejectedCOMMS(0);
    inline Parameter<uint16_t> commsInvalidHMAC(0);
    inline Parameter<uint16_t> commsInvalidPacketStructure(0);
    inline Parameter<uint16_t> commsInvalidSpacecraftID(0);
    inline Parameter<uint16_t> commsFrameSequenceCounter(0);

    inline Parameter<float> commsPCBTemperature1(0);
    inline Parameter<float> commsPCBTemperature2(0);
    inline Parameter<float> commsMCUTemperature(0);
    inline Parameter<float> commsMCUInputVoltage(0);

    inline Parameter<uint32_t> commsMCUBootCounter(0);
    inline Parameter<Time::DefaultCUC> commsOnBoardTime(Time::DefaultCUC(0));

    inline Parameter<MemoryPartition> commsNANDCurrentlyUsedMemoryPartition(First); // enum

    inline Parameter<uint16_t> commsLastFailedEvent(0);
    inline Parameter<uint32_t> commsMCUSystick(0);
    inline Parameter<uint32_t> commsFlashInt(0);
    inline Parameter<uint32_t> commsSRAMInt(0);
} // namespace AcubeSATParameters

#pragma GCC diagnostic pop