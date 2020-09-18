#if defined(UNIT_TEST)
#include "testBms.h"
#include <unity.h>
#include <Bms.h>

void testSoftwareVersion(){
    SoftwareVersion version;
    TEST_ASSERT_EQUAL(0, version.major);
    TEST_ASSERT_EQUAL(0, version.minor);
}

void testSoftwareVersionFromUint8_t(){
    SoftwareVersion version = SoftwareVersion(0x10);
    TEST_ASSERT_EQUAL(1, version.major);
    TEST_ASSERT_EQUAL(0, version.minor);
}

void testProductionDate(){
    ProductionDate date;
    TEST_ASSERT_EQUAL(1, date.day);
    TEST_ASSERT_EQUAL(1, date.month);
    TEST_ASSERT_EQUAL(2000, date.year);
}

void testProductionDateFromUint16_t(){
    ProductionDate date = ProductionDate(0x2068);
    TEST_ASSERT_EQUAL(8, date.day);
    TEST_ASSERT_EQUAL(3, date.month);
    TEST_ASSERT_EQUAL(2016, date.year);
}

void testProtectionStatus() {
    ProtectionStatus status;
    TEST_ASSERT_EQUAL(false, status.singleCellOvervoltageProtection);
    TEST_ASSERT_EQUAL(false, status.singleCellUndervoltageProtection);
    TEST_ASSERT_EQUAL(false, status.wholePackOvervoltageProtection);
    TEST_ASSERT_EQUAL(false, status.wholePackUndervoltageProtection);
    TEST_ASSERT_EQUAL(false, status.chargingOverTemperatureProtection);
    TEST_ASSERT_EQUAL(false, status.chargingLowTemperatureProtection);
    TEST_ASSERT_EQUAL(false, status.dischargeOverTemperatureProtection);
    TEST_ASSERT_EQUAL(false, status.dischargeLowTemperatureProtection);
    TEST_ASSERT_EQUAL(false, status.chargingOvercurrentProtection);
    TEST_ASSERT_EQUAL(false, status.dischargeOvercurrentProtection);
    TEST_ASSERT_EQUAL(false, status.shortCircuitProtection);
    TEST_ASSERT_EQUAL(false, status.frontEndDetectionIcError);
    TEST_ASSERT_EQUAL(false, status.softwareLockMos);
}

void testProtectionStatusFromUint16_t() {
    ProtectionStatus status = ProtectionStatus(0x1FFF);
    TEST_ASSERT_EQUAL(true, status.singleCellOvervoltageProtection);
    TEST_ASSERT_EQUAL(true, status.singleCellUndervoltageProtection);
    TEST_ASSERT_EQUAL(true, status.wholePackOvervoltageProtection);
    TEST_ASSERT_EQUAL(true, status.wholePackUndervoltageProtection);
    TEST_ASSERT_EQUAL(true, status.chargingOverTemperatureProtection);
    TEST_ASSERT_EQUAL(true, status.chargingLowTemperatureProtection);
    TEST_ASSERT_EQUAL(true, status.dischargeOverTemperatureProtection);
    TEST_ASSERT_EQUAL(true, status.dischargeLowTemperatureProtection);
    TEST_ASSERT_EQUAL(true, status.chargingOvercurrentProtection);
    TEST_ASSERT_EQUAL(true, status.dischargeOvercurrentProtection);
    TEST_ASSERT_EQUAL(true, status.shortCircuitProtection);
    TEST_ASSERT_EQUAL(true, status.frontEndDetectionIcError);
    TEST_ASSERT_EQUAL(true, status.softwareLockMos);
}

void testCalculateChecksumCmdBasicSystemInfo(){
    Bms bms(&Serial1);
    uint8_t *data = bms.basicSystemInfoCommand;
    TEST_ASSERT_EQUAL(0xFFFD, Bms::calculateChecksum(&data[2], 2));
}

void testCalculateChecksumCmdCellVoltages(){
    Bms bms(&Serial1);
    uint8_t *data = bms.cellVoltagesCommand;
    TEST_ASSERT_EQUAL(0xFFFC, Bms::calculateChecksum(&data[2], 2));
}

void testCalculateChecksumCmdName(){
    Bms bms(&Serial1);
    uint8_t *data = bms.nameCommand;
    TEST_ASSERT_EQUAL(0xFFFB, Bms::calculateChecksum(&data[2], 2));
}

void testMosfetCommandStringNoChargeNoDischarge(){
    Bms bms(&Serial1);
    uint8_t data[] = {START_BYTE, WRITE, CMD_CTL_MOSFET, 0x02, 0x00, 0x00, 0x00, 0x00, STOP_BYTE};
    bms.calculateMosfetCommandString(data, false, false);
    TEST_ASSERT_EQUAL_HEX(0xFF1A, Bms::calculateChecksum(&data[2], 4));
}

void testMosfetCommandStringChargeNoDischarge(){
    Bms bms(&Serial1);
    uint8_t data[]  = {START_BYTE, WRITE, CMD_CTL_MOSFET, 0x02, 0x00, 0x00, 0x00, 0x00, STOP_BYTE};
    bms.calculateMosfetCommandString(data, true, false);
    TEST_ASSERT_EQUAL_HEX(0xFF1B, Bms::calculateChecksum(&data[2], 4));
}

void testMosfetCommandStringNoChargeDischarge(){
    Bms bms(&Serial1);
    uint8_t data[]  = {START_BYTE, WRITE, CMD_CTL_MOSFET, 0x02, 0x00, 0x00, 0x00, 0x00, STOP_BYTE};
    bms.calculateMosfetCommandString(data, false, true);
    TEST_ASSERT_EQUAL_HEX(0xFF1C, Bms::calculateChecksum(&data[2], 4));
}

void testMosfetCommandStringChargeDischarge(){
    Bms bms(&Serial1);
    uint8_t data[]  = {START_BYTE, WRITE, CMD_CTL_MOSFET, 0x02, 0x00, 0x00, 0x00, 0x00, STOP_BYTE};
    bms.calculateMosfetCommandString(data, true, true);
    TEST_ASSERT_EQUAL_HEX(0xFF1D, Bms::calculateChecksum(&data[2], 4));
}

void testValidateResponse(){
    Bms bms(&Serial1);
    uint8_t data[]  = {0xDD, 0x03, 0x00, 0x1B, 0x17, 0x00, 0x00, 0x00, 0x02, 0xD0, 0x03, 0xE8, 0x00, 0x00, 0x20, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x48, 0x03, 0x0F, 0x02, 0x0B, 0x76, 0x0B, 0x82, 0xFB, 0xFF};
    TEST_ASSERT_EQUAL(true, bms.validateResponse(data, 0x03, sizeof(data)));
}

void testBasicInfoResponse(){
    Bms bms(&Serial1);
    uint8_t data[]  = {0xDD, 0x03, 0x00, 0x1B, 0x17, 0x00, 0x00, 0x00, 0x02, 0xD0, 0x03, 0xE8, 0x00, 0x00, 0x20, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x48, 0x03, 0x0F, 0x02, 0x0B, 0x76, 0x0B, 0x82, 0xFB, 0xFF};
    bms.parseBasicInfoResponse(data);
    TEST_ASSERT_EQUAL_FLOAT(58.88, bms.totalVoltage);
    TEST_ASSERT_EQUAL_FLOAT(0.0, bms.current);
    TEST_ASSERT_EQUAL_FLOAT(7.20, bms.balanceCapacity);
    TEST_ASSERT_EQUAL_FLOAT(10.0, bms.rateCapacity);
    TEST_ASSERT_EQUAL(0, bms.cycleCount);
    TEST_ASSERT_EQUAL(24, bms.productionDate.day);
    TEST_ASSERT_EQUAL(3, bms.productionDate.month);
    TEST_ASSERT_EQUAL(2016, bms.productionDate.year);
    for (int i = 0; i < bms.numCells; i++) {
        TEST_ASSERT_EQUAL(false, bms.isBalancing(i));
    }

    TEST_ASSERT_EQUAL(0, bms.protectionStatus.singleCellOvervoltageProtectionCount);
    TEST_ASSERT_EQUAL(0, bms.protectionStatus.singleCellUndervoltageProtection);
    TEST_ASSERT_EQUAL(0, bms.protectionStatus.wholePackOvervoltageProtection);
    TEST_ASSERT_EQUAL(0, bms.protectionStatus.wholePackUndervoltageProtection);
    TEST_ASSERT_EQUAL(0, bms.protectionStatus.chargingOverTemperatureProtection);
    TEST_ASSERT_EQUAL(0, bms.protectionStatus.chargingLowTemperatureProtection);
    TEST_ASSERT_EQUAL(0, bms.protectionStatus.dischargeOverTemperatureProtection);
    TEST_ASSERT_EQUAL(0, bms.protectionStatus.dischargeLowTemperatureProtection);
    TEST_ASSERT_EQUAL(0, bms.protectionStatus.chargingOvercurrentProtection);
    TEST_ASSERT_EQUAL(0, bms.protectionStatus.dischargeOvercurrentProtection);
    TEST_ASSERT_EQUAL(0, bms.protectionStatus.shortCircuitProtection);
    TEST_ASSERT_EQUAL(0, bms.protectionStatus.frontEndDetectionIcError);
    TEST_ASSERT_EQUAL(0, bms.protectionStatus.softwareLockMos);
    TEST_ASSERT_EQUAL(false, bms.protectionStatus.singleCellOvervoltageProtection);
    TEST_ASSERT_EQUAL(false, bms.protectionStatus.singleCellUndervoltageProtection);
    TEST_ASSERT_EQUAL(false, bms.protectionStatus.wholePackOvervoltageProtection);
    TEST_ASSERT_EQUAL(false, bms.protectionStatus.wholePackUndervoltageProtection);
    TEST_ASSERT_EQUAL(false, bms.protectionStatus.chargingOverTemperatureProtection);
    TEST_ASSERT_EQUAL(false, bms.protectionStatus.chargingLowTemperatureProtection);
    TEST_ASSERT_EQUAL(false, bms.protectionStatus.dischargeOverTemperatureProtection);
    TEST_ASSERT_EQUAL(false, bms.protectionStatus.dischargeLowTemperatureProtection);
    TEST_ASSERT_EQUAL(false, bms.protectionStatus.chargingOvercurrentProtection);
    TEST_ASSERT_EQUAL(false, bms.protectionStatus.dischargeOvercurrentProtection);
    TEST_ASSERT_EQUAL(false, bms.protectionStatus.shortCircuitProtection);
    TEST_ASSERT_EQUAL(false, bms.protectionStatus.frontEndDetectionIcError);
    TEST_ASSERT_EQUAL(false, bms.protectionStatus.softwareLockMos);
    TEST_ASSERT_EQUAL(1, bms.softwareVersion.major);
    TEST_ASSERT_EQUAL(0, bms.softwareVersion.minor);
    TEST_ASSERT_EQUAL(72, bms.stateOfCharge);
    TEST_ASSERT_EQUAL(true, bms.isDischargeFetEnabled);
    TEST_ASSERT_EQUAL(true, bms.isChargeFetEnabled);
    TEST_ASSERT_EQUAL(15, bms.numCells);
    TEST_ASSERT_EQUAL(2, bms.numTemperatureSensors);
    TEST_ASSERT_EQUAL_FLOAT(20.3, bms.temperatures[0]);
    TEST_ASSERT_EQUAL_FLOAT(21.5, bms.temperatures[1]);
}

void testVoltagesResponse(){
    Bms bms(&Serial1);
    bms.numCells = 15;
    uint8_t data[]  = {0xDD, 0x04, 0x00, 0x1E, 0x0F, 0x66, 0x0F, 0x63, 0x0F, 0x63, 0x0F, 0x64, 0x0F, 0x3E, 0x0F, 0x63, 0x0F, 0x37, 0x0F, 0x5B, 0x0F, 0x65, 0x0F, 0x3B, 0x0F, 0x63, 0x0F, 0x63, 0x0F, 0x3C, 0x0F, 0x66, 0x0F, 0x3D, 0xF9, 0xF9};
    bms.parseVoltagesResponse(data);
    TEST_ASSERT_EQUAL_FLOAT(3.942, bms.cellVoltages[0]);
    TEST_ASSERT_EQUAL_FLOAT(3.939, bms.cellVoltages[1]);
    TEST_ASSERT_EQUAL_FLOAT(3.939, bms.cellVoltages[2]);
    TEST_ASSERT_EQUAL_FLOAT(3.940, bms.cellVoltages[3]);
    TEST_ASSERT_EQUAL_FLOAT(3.902, bms.cellVoltages[4]);
    TEST_ASSERT_EQUAL_FLOAT(3.939, bms.cellVoltages[5]);
    TEST_ASSERT_EQUAL_FLOAT(3.895, bms.cellVoltages[6]);
    TEST_ASSERT_EQUAL_FLOAT(3.931, bms.cellVoltages[7]);
}

void testNameResponse(){
    Bms bms(&Serial1);
    uint8_t data[]  = {0xDD, 0x05, 0x00, 0x0A, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0xFD, 0xE9};
    bms.parseNameResponse(data, 0x0A);
    TEST_ASSERT_EQUAL_STRING("0123456789", bms.name);
}

void testBmsAll(){
    RUN_TEST(testSoftwareVersion);
    RUN_TEST(testSoftwareVersionFromUint8_t);
    RUN_TEST(testProductionDate);
    RUN_TEST(testProductionDateFromUint16_t);
    RUN_TEST(testProtectionStatus);
    RUN_TEST(testProtectionStatusFromUint16_t);
    RUN_TEST(testCalculateChecksumCmdBasicSystemInfo);
    RUN_TEST(testCalculateChecksumCmdCellVoltages);
    RUN_TEST(testCalculateChecksumCmdName);
    RUN_TEST(testMosfetCommandStringNoChargeNoDischarge);
    RUN_TEST(testMosfetCommandStringChargeNoDischarge);
    RUN_TEST(testMosfetCommandStringNoChargeDischarge);
    RUN_TEST(testMosfetCommandStringChargeDischarge);
    RUN_TEST(testValidateResponse);
    RUN_TEST(testBasicInfoResponse);
    RUN_TEST(testVoltagesResponse);
    RUN_TEST(testNameResponse);
}

#endif