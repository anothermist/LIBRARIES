-- ------------------------------------------------------------------------- 
-- Altera DSP Builder Advanced Flow Tools Release Version 15.1
-- Quartus Prime development tool and MATLAB/Simulink Interface
-- 
-- Legal Notice: Copyright 2015 Altera Corporation.  All rights reserved.
-- Your use of  Altera  Corporation's design tools,  logic functions and other
-- software and tools,  and its AMPP  partner logic functions, and  any output
-- files  any of the  foregoing  device programming or simulation files),  and
-- any associated  documentation or information are expressly subject  to  the
-- terms and conditions  of the Altera Program License Subscription Agreement,
-- Altera  MegaCore  Function  License  Agreement, or other applicable license
-- agreement,  including,  without limitation,  that your use  is for the sole
-- purpose of  programming  logic  devices  manufactured by Altera and sold by
-- Altera or its authorized  distributors.  Please  refer  to  the  applicable
-- agreement for further details.
-- ---------------------------------------------------------------------------

-- VHDL created from xlr8_float_mult2_0002
-- VHDL created on Tue Mar 29 15:09:51 2016


library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.NUMERIC_STD.all;
use IEEE.MATH_REAL.all;
use std.TextIO.all;
use work.dspba_library_package.all;

LIBRARY altera_mf;
USE altera_mf.altera_mf_components.all;
LIBRARY lpm;
USE lpm.lpm_components.all;

entity xlr8_float_mult2_0002 is
    port (
        a : in std_logic_vector(31 downto 0);  -- float32_m23
        b : in std_logic_vector(31 downto 0);  -- float32_m23
        en : in std_logic_vector(0 downto 0);  -- ufix1
        q : out std_logic_vector(31 downto 0);  -- float32_m23
        clk : in std_logic;
        areset : in std_logic
    );
end xlr8_float_mult2_0002;

architecture normal of xlr8_float_mult2_0002 is

    attribute altera_attribute : string;
    attribute altera_attribute of normal : architecture is "-name PHYSICAL_SYNTHESIS_REGISTER_DUPLICATION ON; -name AUTO_SHIFT_REGISTER_RECOGNITION OFF; -name MESSAGE_DISABLE 10036; -name MESSAGE_DISABLE 10037; -name MESSAGE_DISABLE 14130; -name MESSAGE_DISABLE 14320; -name MESSAGE_DISABLE 15400; -name MESSAGE_DISABLE 14130; -name MESSAGE_DISABLE 10036; -name MESSAGE_DISABLE 12020; -name MESSAGE_DISABLE 12030; -name MESSAGE_DISABLE 12010; -name MESSAGE_DISABLE 12110; -name MESSAGE_DISABLE 14320; -name MESSAGE_DISABLE 13410; -name MESSAGE_DISABLE 113007";
    
    signal GND_q : STD_LOGIC_VECTOR (0 downto 0);
    signal VCC_q : STD_LOGIC_VECTOR (0 downto 0);
    signal cstAllOWE_uid10_fpMulTest_q : STD_LOGIC_VECTOR (7 downto 0);
    signal cstZeroWF_uid11_fpMulTest_q : STD_LOGIC_VECTOR (22 downto 0);
    signal cstAllZWE_uid12_fpMulTest_q : STD_LOGIC_VECTOR (7 downto 0);
    signal excZ_x_uid15_fpMulTest_a : STD_LOGIC_VECTOR (7 downto 0);
    signal excZ_x_uid15_fpMulTest_b : STD_LOGIC_VECTOR (7 downto 0);
    signal excZ_x_uid15_fpMulTest_q_i : STD_LOGIC_VECTOR (0 downto 0);
    signal excZ_x_uid15_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal expXIsMax_uid16_fpMulTest_a : STD_LOGIC_VECTOR (7 downto 0);
    signal expXIsMax_uid16_fpMulTest_b : STD_LOGIC_VECTOR (7 downto 0);
    signal expXIsMax_uid16_fpMulTest_q_i : STD_LOGIC_VECTOR (0 downto 0);
    signal expXIsMax_uid16_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal fracXIsZero_uid17_fpMulTest_a : STD_LOGIC_VECTOR (22 downto 0);
    signal fracXIsZero_uid17_fpMulTest_b : STD_LOGIC_VECTOR (22 downto 0);
    signal fracXIsZero_uid17_fpMulTest_q_i : STD_LOGIC_VECTOR (0 downto 0);
    signal fracXIsZero_uid17_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excZ_y_uid29_fpMulTest_a : STD_LOGIC_VECTOR (7 downto 0);
    signal excZ_y_uid29_fpMulTest_b : STD_LOGIC_VECTOR (7 downto 0);
    signal excZ_y_uid29_fpMulTest_q_i : STD_LOGIC_VECTOR (0 downto 0);
    signal excZ_y_uid29_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal expXIsMax_uid30_fpMulTest_a : STD_LOGIC_VECTOR (7 downto 0);
    signal expXIsMax_uid30_fpMulTest_b : STD_LOGIC_VECTOR (7 downto 0);
    signal expXIsMax_uid30_fpMulTest_q_i : STD_LOGIC_VECTOR (0 downto 0);
    signal expXIsMax_uid30_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal fracXIsZero_uid31_fpMulTest_a : STD_LOGIC_VECTOR (22 downto 0);
    signal fracXIsZero_uid31_fpMulTest_b : STD_LOGIC_VECTOR (22 downto 0);
    signal fracXIsZero_uid31_fpMulTest_q_i : STD_LOGIC_VECTOR (0 downto 0);
    signal fracXIsZero_uid31_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal expSum_uid44_fpMulTest_a : STD_LOGIC_VECTOR (8 downto 0);
    signal expSum_uid44_fpMulTest_b : STD_LOGIC_VECTOR (8 downto 0);
    signal expSum_uid44_fpMulTest_o : STD_LOGIC_VECTOR (8 downto 0);
    signal expSum_uid44_fpMulTest_q : STD_LOGIC_VECTOR (8 downto 0);
    signal biasInc_uid45_fpMulTest_q : STD_LOGIC_VECTOR (9 downto 0);
    signal signR_uid48_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal signR_uid48_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal signR_uid48_fpMulTest_q_i : STD_LOGIC_VECTOR (0 downto 0);
    signal signR_uid48_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal roundBitDetectionConstant_uid63_fpMulTest_q : STD_LOGIC_VECTOR (2 downto 0);
    signal oneFracRPostExc2_uid92_fpMulTest_q : STD_LOGIC_VECTOR (22 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im0_a0 : STD_LOGIC_VECTOR (17 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im0_b0 : STD_LOGIC_VECTOR (17 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im0_s1 : STD_LOGIC_VECTOR (35 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im0_reset : std_logic;
    signal prodXY_uid105_prod_uid47_fpMulTest_im0_q : STD_LOGIC_VECTOR (35 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im3_a0 : STD_LOGIC_VECTOR (17 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im3_b0 : STD_LOGIC_VECTOR (5 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im3_s1 : STD_LOGIC_VECTOR (23 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im3_reset : std_logic;
    signal prodXY_uid105_prod_uid47_fpMulTest_im3_q : STD_LOGIC_VECTOR (23 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im6_a0 : STD_LOGIC_VECTOR (17 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im6_b0 : STD_LOGIC_VECTOR (5 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im6_s1 : STD_LOGIC_VECTOR (23 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im6_reset : std_logic;
    signal prodXY_uid105_prod_uid47_fpMulTest_im6_q : STD_LOGIC_VECTOR (23 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im9_a0 : STD_LOGIC_VECTOR (5 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im9_b0 : STD_LOGIC_VECTOR (5 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im9_s1 : STD_LOGIC_VECTOR (11 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_im9_reset : std_logic;
    signal prodXY_uid105_prod_uid47_fpMulTest_im9_q : STD_LOGIC_VECTOR (11 downto 0);
    signal redist0_q : STD_LOGIC_VECTOR (0 downto 0);
    signal redist1_q : STD_LOGIC_VECTOR (8 downto 0);
    signal redist2_q : STD_LOGIC_VECTOR (0 downto 0);
    signal redist3_q : STD_LOGIC_VECTOR (0 downto 0);
    signal redist4_q : STD_LOGIC_VECTOR (0 downto 0);
    signal redist5_q : STD_LOGIC_VECTOR (0 downto 0);
    signal redist6_q : STD_LOGIC_VECTOR (0 downto 0);
    signal redist7_q : STD_LOGIC_VECTOR (0 downto 0);
    signal expX_uid6_fpMulTest_in : STD_LOGIC_VECTOR (31 downto 0);
    signal expX_uid6_fpMulTest_b : STD_LOGIC_VECTOR (7 downto 0);
    signal signX_uid8_fpMulTest_in : STD_LOGIC_VECTOR (31 downto 0);
    signal signX_uid8_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal frac_x_uid14_fpMulTest_in : STD_LOGIC_VECTOR (31 downto 0);
    signal frac_x_uid14_fpMulTest_b : STD_LOGIC_VECTOR (22 downto 0);
    signal expY_uid7_fpMulTest_in : STD_LOGIC_VECTOR (31 downto 0);
    signal expY_uid7_fpMulTest_b : STD_LOGIC_VECTOR (7 downto 0);
    signal signY_uid9_fpMulTest_in : STD_LOGIC_VECTOR (31 downto 0);
    signal signY_uid9_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal frac_y_uid28_fpMulTest_in : STD_LOGIC_VECTOR (31 downto 0);
    signal frac_y_uid28_fpMulTest_b : STD_LOGIC_VECTOR (22 downto 0);
    signal fracXIsNotZero_uid18_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal fracXIsNotZero_uid18_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excI_x_uid19_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excI_x_uid19_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excI_x_uid19_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excN_x_uid20_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excN_x_uid20_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excN_x_uid20_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal invExpXIsMax_uid21_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal invExpXIsMax_uid21_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal InvExpXIsZero_uid22_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal InvExpXIsZero_uid22_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excR_x_uid23_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excR_x_uid23_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excR_x_uid23_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal fracXIsNotZero_uid32_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal fracXIsNotZero_uid32_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excI_y_uid33_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excI_y_uid33_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excI_y_uid33_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excN_y_uid34_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excN_y_uid34_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excN_y_uid34_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal invExpXIsMax_uid35_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal invExpXIsMax_uid35_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal InvExpXIsZero_uid36_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal InvExpXIsZero_uid36_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excR_y_uid37_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excR_y_uid37_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excR_y_uid37_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal expSumMBias_uid46_fpMulTest_a : STD_LOGIC_VECTOR (11 downto 0);
    signal expSumMBias_uid46_fpMulTest_b : STD_LOGIC_VECTOR (11 downto 0);
    signal expSumMBias_uid46_fpMulTest_o : STD_LOGIC_VECTOR (11 downto 0);
    signal expSumMBias_uid46_fpMulTest_q : STD_LOGIC_VECTOR (10 downto 0);
    signal excXZAndExcYZ_uid76_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excXZAndExcYZ_uid76_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excXZAndExcYZ_uid76_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excXZAndExcYR_uid77_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excXZAndExcYR_uid77_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excXZAndExcYR_uid77_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excYZAndExcXR_uid78_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excYZAndExcXR_uid78_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excYZAndExcXR_uid78_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excXIAndExcYI_uid81_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excXIAndExcYI_uid81_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excXIAndExcYI_uid81_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excXRAndExcYI_uid82_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excXRAndExcYI_uid82_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excXRAndExcYI_uid82_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excYRAndExcXI_uid83_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excYRAndExcXI_uid83_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excYRAndExcXI_uid83_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excYZAndExcXI_uid86_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excYZAndExcXI_uid86_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excYZAndExcXI_uid86_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excXZAndExcYI_uid87_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excXZAndExcYI_uid87_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excXZAndExcYI_uid87_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal ZeroTimesInf_uid88_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal ZeroTimesInf_uid88_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal ZeroTimesInf_uid88_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excRNaN_uid89_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excRNaN_uid89_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excRNaN_uid89_fpMulTest_c : STD_LOGIC_VECTOR (0 downto 0);
    signal excRNaN_uid89_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal invExcRNaN_uid101_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal invExcRNaN_uid101_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal signRPostExc_uid102_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal signRPostExc_uid102_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal signRPostExc_uid102_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_join_12_q : STD_LOGIC_VECTOR (47 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_align_13_q : STD_LOGIC_VECTOR (41 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_align_13_q_int : STD_LOGIC_VECTOR (41 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_align_15_q : STD_LOGIC_VECTOR (41 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_align_15_q_int : STD_LOGIC_VECTOR (41 downto 0);
    signal ofracX_uid40_fpMulTest_q : STD_LOGIC_VECTOR (23 downto 0);
    signal ofracY_uid43_fpMulTest_q : STD_LOGIC_VECTOR (23 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_result_add_0_0_a : STD_LOGIC_VECTOR (48 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_result_add_0_0_b : STD_LOGIC_VECTOR (48 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_result_add_0_0_o : STD_LOGIC_VECTOR (48 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_result_add_0_0_q : STD_LOGIC_VECTOR (48 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_result_add_1_0_a : STD_LOGIC_VECTOR (49 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_result_add_1_0_b : STD_LOGIC_VECTOR (49 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_result_add_1_0_o : STD_LOGIC_VECTOR (49 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_result_add_1_0_q : STD_LOGIC_VECTOR (49 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_bs1_in : STD_LOGIC_VECTOR (17 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_bs1_b : STD_LOGIC_VECTOR (17 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_bs7_in : STD_LOGIC_VECTOR (23 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_bs7_b : STD_LOGIC_VECTOR (5 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_bs2_in : STD_LOGIC_VECTOR (17 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_bs2_b : STD_LOGIC_VECTOR (17 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_bs5_in : STD_LOGIC_VECTOR (23 downto 0);
    signal prodXY_uid105_prod_uid47_fpMulTest_bs5_b : STD_LOGIC_VECTOR (5 downto 0);
    signal osig_uid106_prod_uid47_fpMulTest_in : STD_LOGIC_VECTOR (47 downto 0);
    signal osig_uid106_prod_uid47_fpMulTest_b : STD_LOGIC_VECTOR (47 downto 0);
    signal normalizeBit_uid49_fpMulTest_in : STD_LOGIC_VECTOR (47 downto 0);
    signal normalizeBit_uid49_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal fracRPostNormHigh_uid51_fpMulTest_in : STD_LOGIC_VECTOR (46 downto 0);
    signal fracRPostNormHigh_uid51_fpMulTest_b : STD_LOGIC_VECTOR (23 downto 0);
    signal fracRPostNormLow_uid52_fpMulTest_in : STD_LOGIC_VECTOR (45 downto 0);
    signal fracRPostNormLow_uid52_fpMulTest_b : STD_LOGIC_VECTOR (23 downto 0);
    signal stickyRange_uid54_fpMulTest_in : STD_LOGIC_VECTOR (21 downto 0);
    signal stickyRange_uid54_fpMulTest_b : STD_LOGIC_VECTOR (21 downto 0);
    signal extraStickyBitOfProd_uid55_fpMulTest_in : STD_LOGIC_VECTOR (22 downto 0);
    signal extraStickyBitOfProd_uid55_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal fracRPostNorm_uid53_fpMulTest_s : STD_LOGIC_VECTOR (0 downto 0);
    signal fracRPostNorm_uid53_fpMulTest_q : STD_LOGIC_VECTOR (23 downto 0);
    signal extraStickyBit_uid56_fpMulTest_s : STD_LOGIC_VECTOR (0 downto 0);
    signal extraStickyBit_uid56_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal stickyExtendedRange_uid57_fpMulTest_q : STD_LOGIC_VECTOR (22 downto 0);
    signal fracRPostNorm1dto0_uid61_fpMulTest_in : STD_LOGIC_VECTOR (1 downto 0);
    signal fracRPostNorm1dto0_uid61_fpMulTest_b : STD_LOGIC_VECTOR (1 downto 0);
    signal expFracPreRound_uid66_fpMulTest_q : STD_LOGIC_VECTOR (34 downto 0);
    signal stickyRangeComparator_uid59_fpMulTest_a : STD_LOGIC_VECTOR (22 downto 0);
    signal stickyRangeComparator_uid59_fpMulTest_b : STD_LOGIC_VECTOR (22 downto 0);
    signal stickyRangeComparator_uid59_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal sticky_uid60_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal sticky_uid60_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal lrs_uid62_fpMulTest_q : STD_LOGIC_VECTOR (2 downto 0);
    signal roundBitDetectionPattern_uid64_fpMulTest_a : STD_LOGIC_VECTOR (2 downto 0);
    signal roundBitDetectionPattern_uid64_fpMulTest_b : STD_LOGIC_VECTOR (2 downto 0);
    signal roundBitDetectionPattern_uid64_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal roundBit_uid65_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal roundBit_uid65_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal roundBitAndNormalizationOp_uid68_fpMulTest_q : STD_LOGIC_VECTOR (25 downto 0);
    signal expFracRPostRounding_uid69_fpMulTest_a : STD_LOGIC_VECTOR (36 downto 0);
    signal expFracRPostRounding_uid69_fpMulTest_b : STD_LOGIC_VECTOR (36 downto 0);
    signal expFracRPostRounding_uid69_fpMulTest_o : STD_LOGIC_VECTOR (36 downto 0);
    signal expFracRPostRounding_uid69_fpMulTest_q : STD_LOGIC_VECTOR (35 downto 0);
    signal fracRPreExc_uid70_fpMulTest_in : STD_LOGIC_VECTOR (23 downto 0);
    signal fracRPreExc_uid70_fpMulTest_b : STD_LOGIC_VECTOR (22 downto 0);
    signal expRPreExcExt_uid71_fpMulTest_in : STD_LOGIC_VECTOR (35 downto 0);
    signal expRPreExcExt_uid71_fpMulTest_b : STD_LOGIC_VECTOR (11 downto 0);
    signal expRPreExc_uid72_fpMulTest_in : STD_LOGIC_VECTOR (7 downto 0);
    signal expRPreExc_uid72_fpMulTest_b : STD_LOGIC_VECTOR (7 downto 0);
    signal expUdf_uid73_fpMulTest_a : STD_LOGIC_VECTOR (14 downto 0);
    signal expUdf_uid73_fpMulTest_b : STD_LOGIC_VECTOR (14 downto 0);
    signal expUdf_uid73_fpMulTest_o : STD_LOGIC_VECTOR (14 downto 0);
    signal expUdf_uid73_fpMulTest_cin : STD_LOGIC_VECTOR (0 downto 0);
    signal expUdf_uid73_fpMulTest_n : STD_LOGIC_VECTOR (0 downto 0);
    signal expOvf_uid75_fpMulTest_a : STD_LOGIC_VECTOR (14 downto 0);
    signal expOvf_uid75_fpMulTest_b : STD_LOGIC_VECTOR (14 downto 0);
    signal expOvf_uid75_fpMulTest_o : STD_LOGIC_VECTOR (14 downto 0);
    signal expOvf_uid75_fpMulTest_cin : STD_LOGIC_VECTOR (0 downto 0);
    signal expOvf_uid75_fpMulTest_n : STD_LOGIC_VECTOR (0 downto 0);
    signal excZC3_uid79_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excZC3_uid79_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excZC3_uid79_fpMulTest_c : STD_LOGIC_VECTOR (0 downto 0);
    signal excZC3_uid79_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal ExcROvfAndInReg_uid84_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal ExcROvfAndInReg_uid84_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal ExcROvfAndInReg_uid84_fpMulTest_c : STD_LOGIC_VECTOR (0 downto 0);
    signal ExcROvfAndInReg_uid84_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excRZero_uid80_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excRZero_uid80_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excRZero_uid80_fpMulTest_c : STD_LOGIC_VECTOR (0 downto 0);
    signal excRZero_uid80_fpMulTest_d : STD_LOGIC_VECTOR (0 downto 0);
    signal excRZero_uid80_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal excRInf_uid85_fpMulTest_a : STD_LOGIC_VECTOR (0 downto 0);
    signal excRInf_uid85_fpMulTest_b : STD_LOGIC_VECTOR (0 downto 0);
    signal excRInf_uid85_fpMulTest_c : STD_LOGIC_VECTOR (0 downto 0);
    signal excRInf_uid85_fpMulTest_d : STD_LOGIC_VECTOR (0 downto 0);
    signal excRInf_uid85_fpMulTest_q : STD_LOGIC_VECTOR (0 downto 0);
    signal concExc_uid90_fpMulTest_q : STD_LOGIC_VECTOR (2 downto 0);
    signal excREnc_uid91_fpMulTest_q : STD_LOGIC_VECTOR (1 downto 0);
    signal fracRPostExc_uid95_fpMulTest_s : STD_LOGIC_VECTOR (1 downto 0);
    signal fracRPostExc_uid95_fpMulTest_q : STD_LOGIC_VECTOR (22 downto 0);
    signal expRPostExc_uid100_fpMulTest_s : STD_LOGIC_VECTOR (1 downto 0);
    signal expRPostExc_uid100_fpMulTest_q : STD_LOGIC_VECTOR (7 downto 0);
    signal R_uid103_fpMulTest_q : STD_LOGIC_VECTOR (31 downto 0);

begin


    -- frac_x_uid14_fpMulTest(BITSELECT,13)@0
    frac_x_uid14_fpMulTest_in <= a;
    frac_x_uid14_fpMulTest_b <= frac_x_uid14_fpMulTest_in(22 downto 0);

    -- cstZeroWF_uid11_fpMulTest(CONSTANT,10)
    cstZeroWF_uid11_fpMulTest_q <= "00000000000000000000000";

    -- fracXIsZero_uid17_fpMulTest(LOGICAL,16)@0
    fracXIsZero_uid17_fpMulTest_a <= cstZeroWF_uid11_fpMulTest_q;
    fracXIsZero_uid17_fpMulTest_b <= frac_x_uid14_fpMulTest_b;
    fracXIsZero_uid17_fpMulTest_q_i <= "1" WHEN fracXIsZero_uid17_fpMulTest_a = fracXIsZero_uid17_fpMulTest_b ELSE "0";
    fracXIsZero_uid17_fpMulTest_delay : dspba_delay
    GENERIC MAP ( width => 1, depth => 1, reset_kind => "ASYNC" )
    PORT MAP ( xin => fracXIsZero_uid17_fpMulTest_q_i, xout => fracXIsZero_uid17_fpMulTest_q, ena => en(0), clk => clk, aclr => areset );

    -- redist5(DELAY,131)
    redist5 : dspba_delay
    GENERIC MAP ( width => 1, depth => 1, reset_kind => "ASYNC" )
    PORT MAP ( xin => fracXIsZero_uid17_fpMulTest_q, xout => redist5_q, ena => en(0), clk => clk, aclr => areset );

    -- cstAllOWE_uid10_fpMulTest(CONSTANT,9)
    cstAllOWE_uid10_fpMulTest_q <= "11111111";

    -- expX_uid6_fpMulTest(BITSELECT,5)@0
    expX_uid6_fpMulTest_in <= a;
    expX_uid6_fpMulTest_b <= expX_uid6_fpMulTest_in(30 downto 23);

    -- expXIsMax_uid16_fpMulTest(LOGICAL,15)@0
    expXIsMax_uid16_fpMulTest_a <= expX_uid6_fpMulTest_b;
    expXIsMax_uid16_fpMulTest_b <= cstAllOWE_uid10_fpMulTest_q;
    expXIsMax_uid16_fpMulTest_q_i <= "1" WHEN expXIsMax_uid16_fpMulTest_a = expXIsMax_uid16_fpMulTest_b ELSE "0";
    expXIsMax_uid16_fpMulTest_delay : dspba_delay
    GENERIC MAP ( width => 1, depth => 1, reset_kind => "ASYNC" )
    PORT MAP ( xin => expXIsMax_uid16_fpMulTest_q_i, xout => expXIsMax_uid16_fpMulTest_q, ena => en(0), clk => clk, aclr => areset );

    -- redist6(DELAY,132)
    redist6 : dspba_delay
    GENERIC MAP ( width => 1, depth => 1, reset_kind => "ASYNC" )
    PORT MAP ( xin => expXIsMax_uid16_fpMulTest_q, xout => redist6_q, ena => en(0), clk => clk, aclr => areset );

    -- excI_x_uid19_fpMulTest(LOGICAL,18)@2
    excI_x_uid19_fpMulTest_a <= redist6_q;
    excI_x_uid19_fpMulTest_b <= redist5_q;
    excI_x_uid19_fpMulTest_q <= excI_x_uid19_fpMulTest_a and excI_x_uid19_fpMulTest_b;

    -- cstAllZWE_uid12_fpMulTest(CONSTANT,11)
    cstAllZWE_uid12_fpMulTest_q <= "00000000";

    -- expY_uid7_fpMulTest(BITSELECT,6)@0
    expY_uid7_fpMulTest_in <= b;
    expY_uid7_fpMulTest_b <= expY_uid7_fpMulTest_in(30 downto 23);

    -- excZ_y_uid29_fpMulTest(LOGICAL,28)@0
    excZ_y_uid29_fpMulTest_a <= expY_uid7_fpMulTest_b;
    excZ_y_uid29_fpMulTest_b <= cstAllZWE_uid12_fpMulTest_q;
    excZ_y_uid29_fpMulTest_q_i <= "1" WHEN excZ_y_uid29_fpMulTest_a = excZ_y_uid29_fpMulTest_b ELSE "0";
    excZ_y_uid29_fpMulTest_delay : dspba_delay
    GENERIC MAP ( width => 1, depth => 1, reset_kind => "ASYNC" )
    PORT MAP ( xin => excZ_y_uid29_fpMulTest_q_i, xout => excZ_y_uid29_fpMulTest_q, ena => en(0), clk => clk, aclr => areset );

    -- redist4(DELAY,130)
    redist4 : dspba_delay
    GENERIC MAP ( width => 1, depth => 1, reset_kind => "ASYNC" )
    PORT MAP ( xin => excZ_y_uid29_fpMulTest_q, xout => redist4_q, ena => en(0), clk => clk, aclr => areset );

    -- excYZAndExcXI_uid86_fpMulTest(LOGICAL,85)@2
    excYZAndExcXI_uid86_fpMulTest_a <= redist4_q;
    excYZAndExcXI_uid86_fpMulTest_b <= excI_x_uid19_fpMulTest_q;
    excYZAndExcXI_uid86_fpMulTest_q <= excYZAndExcXI_uid86_fpMulTest_a and excYZAndExcXI_uid86_fpMulTest_b;

    -- frac_y_uid28_fpMulTest(BITSELECT,27)@0
    frac_y_uid28_fpMulTest_in <= b;
    frac_y_uid28_fpMulTest_b <= frac_y_uid28_fpMulTest_in(22 downto 0);

    -- fracXIsZero_uid31_fpMulTest(LOGICAL,30)@0
    fracXIsZero_uid31_fpMulTest_a <= cstZeroWF_uid11_fpMulTest_q;
    fracXIsZero_uid31_fpMulTest_b <= frac_y_uid28_fpMulTest_b;
    fracXIsZero_uid31_fpMulTest_q_i <= "1" WHEN fracXIsZero_uid31_fpMulTest_a = fracXIsZero_uid31_fpMulTest_b ELSE "0";
    fracXIsZero_uid31_fpMulTest_delay : dspba_delay
    GENERIC MAP ( width => 1, depth => 1, reset_kind => "ASYNC" )
    PORT MAP ( xin => fracXIsZero_uid31_fpMulTest_q_i, xout => fracXIsZero_uid31_fpMulTest_q, ena => en(0), clk => clk, aclr => areset );

    -- redist2(DELAY,128)
    redist2 : dspba_delay
    GENERIC MAP ( width => 1, depth => 1, reset_kind => "ASYNC" )
    PORT MAP ( xin => fracXIsZero_uid31_fpMulTest_q, xout => redist2_q, ena => en(0), clk => clk, aclr => areset );

    -- expXIsMax_uid30_fpMulTest(LOGICAL,29)@0
    expXIsMax_uid30_fpMulTest_a <= expY_uid7_fpMulTest_b;
    expXIsMax_uid30_fpMulTest_b <= cstAllOWE_uid10_fpMulTest_q;
    expXIsMax_uid30_fpMulTest_q_i <= "1" WHEN expXIsMax_uid30_fpMulTest_a = expXIsMax_uid30_fpMulTest_b ELSE "0";
    expXIsMax_uid30_fpMulTest_delay : dspba_delay
    GENERIC MAP ( width => 1, depth => 1, reset_kind => "ASYNC" )
    PORT MAP ( xin => expXIsMax_uid30_fpMulTest_q_i, xout => expXIsMax_uid30_fpMulTest_q, ena => en(0), clk => clk, aclr => areset );

    -- redist3(DELAY,129)
    redist3 : dspba_delay
    GENERIC MAP ( width => 1, depth => 1, reset_kind => "ASYNC" )
    PORT MAP ( xin => expXIsMax_uid30_fpMulTest_q, xout => redist3_q, ena => en(0), clk => clk, aclr => areset );

    -- excI_y_uid33_fpMulTest(LOGICAL,32)@2
    excI_y_uid33_fpMulTest_a <= redist3_q;
    excI_y_uid33_fpMulTest_b <= redist2_q;
    excI_y_uid33_fpMulTest_q <= excI_y_uid33_fpMulTest_a and excI_y_uid33_fpMulTest_b;

    -- excZ_x_uid15_fpMulTest(LOGICAL,14)@0
    excZ_x_uid15_fpMulTest_a <= expX_uid6_fpMulTest_b;
    excZ_x_uid15_fpMulTest_b <= cstAllZWE_uid12_fpMulTest_q;
    excZ_x_uid15_fpMulTest_q_i <= "1" WHEN excZ_x_uid15_fpMulTest_a = excZ_x_uid15_fpMulTest_b ELSE "0";
    excZ_x_uid15_fpMulTest_delay : dspba_delay
    GENERIC MAP ( width => 1, depth => 1, reset_kind => "ASYNC" )
    PORT MAP ( xin => excZ_x_uid15_fpMulTest_q_i, xout => excZ_x_uid15_fpMulTest_q, ena => en(0), clk => clk, aclr => areset );

    -- redist7(DELAY,133)
    redist7 : dspba_delay
    GENERIC MAP ( width => 1, depth => 1, reset_kind => "ASYNC" )
    PORT MAP ( xin => excZ_x_uid15_fpMulTest_q, xout => redist7_q, ena => en(0), clk => clk, aclr => areset );

    -- excXZAndExcYI_uid87_fpMulTest(LOGICAL,86)@2
    excXZAndExcYI_uid87_fpMulTest_a <= redist7_q;
    excXZAndExcYI_uid87_fpMulTest_b <= excI_y_uid33_fpMulTest_q;
    excXZAndExcYI_uid87_fpMulTest_q <= excXZAndExcYI_uid87_fpMulTest_a and excXZAndExcYI_uid87_fpMulTest_b;

    -- ZeroTimesInf_uid88_fpMulTest(LOGICAL,87)@2
    ZeroTimesInf_uid88_fpMulTest_a <= excXZAndExcYI_uid87_fpMulTest_q;
    ZeroTimesInf_uid88_fpMulTest_b <= excYZAndExcXI_uid86_fpMulTest_q;
    ZeroTimesInf_uid88_fpMulTest_q <= ZeroTimesInf_uid88_fpMulTest_a or ZeroTimesInf_uid88_fpMulTest_b;

    -- fracXIsNotZero_uid32_fpMulTest(LOGICAL,31)@2
    fracXIsNotZero_uid32_fpMulTest_a <= redist2_q;
    fracXIsNotZero_uid32_fpMulTest_q <= not (fracXIsNotZero_uid32_fpMulTest_a);

    -- excN_y_uid34_fpMulTest(LOGICAL,33)@2
    excN_y_uid34_fpMulTest_a <= redist3_q;
    excN_y_uid34_fpMulTest_b <= fracXIsNotZero_uid32_fpMulTest_q;
    excN_y_uid34_fpMulTest_q <= excN_y_uid34_fpMulTest_a and excN_y_uid34_fpMulTest_b;

    -- fracXIsNotZero_uid18_fpMulTest(LOGICAL,17)@2
    fracXIsNotZero_uid18_fpMulTest_a <= redist5_q;
    fracXIsNotZero_uid18_fpMulTest_q <= not (fracXIsNotZero_uid18_fpMulTest_a);

    -- excN_x_uid20_fpMulTest(LOGICAL,19)@2
    excN_x_uid20_fpMulTest_a <= redist6_q;
    excN_x_uid20_fpMulTest_b <= fracXIsNotZero_uid18_fpMulTest_q;
    excN_x_uid20_fpMulTest_q <= excN_x_uid20_fpMulTest_a and excN_x_uid20_fpMulTest_b;

    -- excRNaN_uid89_fpMulTest(LOGICAL,88)@2
    excRNaN_uid89_fpMulTest_a <= excN_x_uid20_fpMulTest_q;
    excRNaN_uid89_fpMulTest_b <= excN_y_uid34_fpMulTest_q;
    excRNaN_uid89_fpMulTest_c <= ZeroTimesInf_uid88_fpMulTest_q;
    excRNaN_uid89_fpMulTest_q <= excRNaN_uid89_fpMulTest_a or excRNaN_uid89_fpMulTest_b or excRNaN_uid89_fpMulTest_c;

    -- invExcRNaN_uid101_fpMulTest(LOGICAL,100)@2
    invExcRNaN_uid101_fpMulTest_a <= excRNaN_uid89_fpMulTest_q;
    invExcRNaN_uid101_fpMulTest_q <= not (invExcRNaN_uid101_fpMulTest_a);

    -- signY_uid9_fpMulTest(BITSELECT,8)@0
    signY_uid9_fpMulTest_in <= STD_LOGIC_VECTOR(b);
    signY_uid9_fpMulTest_b <= signY_uid9_fpMulTest_in(31 downto 31);

    -- signX_uid8_fpMulTest(BITSELECT,7)@0
    signX_uid8_fpMulTest_in <= STD_LOGIC_VECTOR(a);
    signX_uid8_fpMulTest_b <= signX_uid8_fpMulTest_in(31 downto 31);

    -- signR_uid48_fpMulTest(LOGICAL,47)@0
    signR_uid48_fpMulTest_a <= signX_uid8_fpMulTest_b;
    signR_uid48_fpMulTest_b <= signY_uid9_fpMulTest_b;
    signR_uid48_fpMulTest_q_i <= signR_uid48_fpMulTest_a xor signR_uid48_fpMulTest_b;
    signR_uid48_fpMulTest_delay : dspba_delay
    GENERIC MAP ( width => 1, depth => 1, reset_kind => "ASYNC" )
    PORT MAP ( xin => signR_uid48_fpMulTest_q_i, xout => signR_uid48_fpMulTest_q, ena => en(0), clk => clk, aclr => areset );

    -- redist0(DELAY,126)
    redist0 : dspba_delay
    GENERIC MAP ( width => 1, depth => 1, reset_kind => "ASYNC" )
    PORT MAP ( xin => signR_uid48_fpMulTest_q, xout => redist0_q, ena => en(0), clk => clk, aclr => areset );

    -- xIn(GPIN,3)@0

    -- signRPostExc_uid102_fpMulTest(LOGICAL,101)@2
    signRPostExc_uid102_fpMulTest_a <= redist0_q;
    signRPostExc_uid102_fpMulTest_b <= invExcRNaN_uid101_fpMulTest_q;
    signRPostExc_uid102_fpMulTest_q <= signRPostExc_uid102_fpMulTest_a and signRPostExc_uid102_fpMulTest_b;

    -- GND(CONSTANT,0)
    GND_q <= "0";

    -- VCC(CONSTANT,1)
    VCC_q <= "1";

    -- ofracX_uid40_fpMulTest(BITJOIN,39)@0
    ofracX_uid40_fpMulTest_q <= VCC_q & frac_x_uid14_fpMulTest_b;

    -- prodXY_uid105_prod_uid47_fpMulTest_bs7(BITSELECT,114)@0
    prodXY_uid105_prod_uid47_fpMulTest_bs7_in <= ofracX_uid40_fpMulTest_q;
    prodXY_uid105_prod_uid47_fpMulTest_bs7_b <= prodXY_uid105_prod_uid47_fpMulTest_bs7_in(23 downto 18);

    -- ofracY_uid43_fpMulTest(BITJOIN,42)@0
    ofracY_uid43_fpMulTest_q <= VCC_q & frac_y_uid28_fpMulTest_b;

    -- prodXY_uid105_prod_uid47_fpMulTest_bs2(BITSELECT,109)@0
    prodXY_uid105_prod_uid47_fpMulTest_bs2_in <= ofracY_uid43_fpMulTest_q(17 downto 0);
    prodXY_uid105_prod_uid47_fpMulTest_bs2_b <= prodXY_uid105_prod_uid47_fpMulTest_bs2_in(17 downto 0);

    -- prodXY_uid105_prod_uid47_fpMulTest_im6(MULT,113)@0
    prodXY_uid105_prod_uid47_fpMulTest_im6_a0 <= prodXY_uid105_prod_uid47_fpMulTest_bs2_b;
    prodXY_uid105_prod_uid47_fpMulTest_im6_b0 <= prodXY_uid105_prod_uid47_fpMulTest_bs7_b;
    prodXY_uid105_prod_uid47_fpMulTest_im6_reset <= areset;
    prodXY_uid105_prod_uid47_fpMulTest_im6_component : lpm_mult
    GENERIC MAP (
        lpm_widtha => 18,
        lpm_widthb => 6,
        lpm_widthp => 24,
        lpm_widths => 1,
        lpm_type => "LPM_MULT",
        lpm_representation => "UNSIGNED",
        lpm_hint => "DEDICATED_MULTIPLIER_CIRCUITRY=YES, MAXIMIZE_SPEED=5",
        lpm_pipeline => 2
    )
    PORT MAP (
        dataa => prodXY_uid105_prod_uid47_fpMulTest_im6_a0,
        datab => prodXY_uid105_prod_uid47_fpMulTest_im6_b0,
        clken => en(0),
        aclr => prodXY_uid105_prod_uid47_fpMulTest_im6_reset,
        clock => clk,
        result => prodXY_uid105_prod_uid47_fpMulTest_im6_s1
    );
    prodXY_uid105_prod_uid47_fpMulTest_im6_q <= prodXY_uid105_prod_uid47_fpMulTest_im6_s1;

    -- prodXY_uid105_prod_uid47_fpMulTest_align_15(BITSHIFT,122)@2
    prodXY_uid105_prod_uid47_fpMulTest_align_15_q_int <= prodXY_uid105_prod_uid47_fpMulTest_im6_q & "000000000000000000";
    prodXY_uid105_prod_uid47_fpMulTest_align_15_q <= prodXY_uid105_prod_uid47_fpMulTest_align_15_q_int(41 downto 0);

    -- prodXY_uid105_prod_uid47_fpMulTest_bs5(BITSELECT,112)@0
    prodXY_uid105_prod_uid47_fpMulTest_bs5_in <= ofracY_uid43_fpMulTest_q;
    prodXY_uid105_prod_uid47_fpMulTest_bs5_b <= prodXY_uid105_prod_uid47_fpMulTest_bs5_in(23 downto 18);

    -- prodXY_uid105_prod_uid47_fpMulTest_bs1(BITSELECT,108)@0
    prodXY_uid105_prod_uid47_fpMulTest_bs1_in <= ofracX_uid40_fpMulTest_q(17 downto 0);
    prodXY_uid105_prod_uid47_fpMulTest_bs1_b <= prodXY_uid105_prod_uid47_fpMulTest_bs1_in(17 downto 0);

    -- prodXY_uid105_prod_uid47_fpMulTest_im3(MULT,110)@0
    prodXY_uid105_prod_uid47_fpMulTest_im3_a0 <= prodXY_uid105_prod_uid47_fpMulTest_bs1_b;
    prodXY_uid105_prod_uid47_fpMulTest_im3_b0 <= prodXY_uid105_prod_uid47_fpMulTest_bs5_b;
    prodXY_uid105_prod_uid47_fpMulTest_im3_reset <= areset;
    prodXY_uid105_prod_uid47_fpMulTest_im3_component : lpm_mult
    GENERIC MAP (
        lpm_widtha => 18,
        lpm_widthb => 6,
        lpm_widthp => 24,
        lpm_widths => 1,
        lpm_type => "LPM_MULT",
        lpm_representation => "UNSIGNED",
        lpm_hint => "DEDICATED_MULTIPLIER_CIRCUITRY=YES, MAXIMIZE_SPEED=5",
        lpm_pipeline => 2
    )
    PORT MAP (
        dataa => prodXY_uid105_prod_uid47_fpMulTest_im3_a0,
        datab => prodXY_uid105_prod_uid47_fpMulTest_im3_b0,
        clken => en(0),
        aclr => prodXY_uid105_prod_uid47_fpMulTest_im3_reset,
        clock => clk,
        result => prodXY_uid105_prod_uid47_fpMulTest_im3_s1
    );
    prodXY_uid105_prod_uid47_fpMulTest_im3_q <= prodXY_uid105_prod_uid47_fpMulTest_im3_s1;

    -- prodXY_uid105_prod_uid47_fpMulTest_align_13(BITSHIFT,120)@2
    prodXY_uid105_prod_uid47_fpMulTest_align_13_q_int <= prodXY_uid105_prod_uid47_fpMulTest_im3_q & "000000000000000000";
    prodXY_uid105_prod_uid47_fpMulTest_align_13_q <= prodXY_uid105_prod_uid47_fpMulTest_align_13_q_int(41 downto 0);

    -- prodXY_uid105_prod_uid47_fpMulTest_im9(MULT,116)@0
    prodXY_uid105_prod_uid47_fpMulTest_im9_a0 <= prodXY_uid105_prod_uid47_fpMulTest_bs7_b;
    prodXY_uid105_prod_uid47_fpMulTest_im9_b0 <= prodXY_uid105_prod_uid47_fpMulTest_bs5_b;
    prodXY_uid105_prod_uid47_fpMulTest_im9_reset <= areset;
    prodXY_uid105_prod_uid47_fpMulTest_im9_component : lpm_mult
    GENERIC MAP (
        lpm_widtha => 6,
        lpm_widthb => 6,
        lpm_widthp => 12,
        lpm_widths => 1,
        lpm_type => "LPM_MULT",
        lpm_representation => "UNSIGNED",
        lpm_hint => "DEDICATED_MULTIPLIER_CIRCUITRY=YES, MAXIMIZE_SPEED=5",
        lpm_pipeline => 2
    )
    PORT MAP (
        dataa => prodXY_uid105_prod_uid47_fpMulTest_im9_a0,
        datab => prodXY_uid105_prod_uid47_fpMulTest_im9_b0,
        clken => en(0),
        aclr => prodXY_uid105_prod_uid47_fpMulTest_im9_reset,
        clock => clk,
        result => prodXY_uid105_prod_uid47_fpMulTest_im9_s1
    );
    prodXY_uid105_prod_uid47_fpMulTest_im9_q <= prodXY_uid105_prod_uid47_fpMulTest_im9_s1;

    -- prodXY_uid105_prod_uid47_fpMulTest_im0(MULT,107)@0
    prodXY_uid105_prod_uid47_fpMulTest_im0_a0 <= prodXY_uid105_prod_uid47_fpMulTest_bs1_b;
    prodXY_uid105_prod_uid47_fpMulTest_im0_b0 <= prodXY_uid105_prod_uid47_fpMulTest_bs2_b;
    prodXY_uid105_prod_uid47_fpMulTest_im0_reset <= areset;
    prodXY_uid105_prod_uid47_fpMulTest_im0_component : lpm_mult
    GENERIC MAP (
        lpm_widtha => 18,
        lpm_widthb => 18,
        lpm_widthp => 36,
        lpm_widths => 1,
        lpm_type => "LPM_MULT",
        lpm_representation => "UNSIGNED",
        lpm_hint => "DEDICATED_MULTIPLIER_CIRCUITRY=YES, MAXIMIZE_SPEED=5",
        lpm_pipeline => 2
    )
    PORT MAP (
        dataa => prodXY_uid105_prod_uid47_fpMulTest_im0_a0,
        datab => prodXY_uid105_prod_uid47_fpMulTest_im0_b0,
        clken => en(0),
        aclr => prodXY_uid105_prod_uid47_fpMulTest_im0_reset,
        clock => clk,
        result => prodXY_uid105_prod_uid47_fpMulTest_im0_s1
    );
    prodXY_uid105_prod_uid47_fpMulTest_im0_q <= prodXY_uid105_prod_uid47_fpMulTest_im0_s1;

    -- prodXY_uid105_prod_uid47_fpMulTest_join_12(BITJOIN,119)@2
    prodXY_uid105_prod_uid47_fpMulTest_join_12_q <= prodXY_uid105_prod_uid47_fpMulTest_im9_q & prodXY_uid105_prod_uid47_fpMulTest_im0_q;

    -- prodXY_uid105_prod_uid47_fpMulTest_result_add_0_0(ADD,124)@2
    prodXY_uid105_prod_uid47_fpMulTest_result_add_0_0_a <= STD_LOGIC_VECTOR("0" & prodXY_uid105_prod_uid47_fpMulTest_join_12_q);
    prodXY_uid105_prod_uid47_fpMulTest_result_add_0_0_b <= STD_LOGIC_VECTOR("0000000" & prodXY_uid105_prod_uid47_fpMulTest_align_13_q);
    prodXY_uid105_prod_uid47_fpMulTest_result_add_0_0_o <= STD_LOGIC_VECTOR(UNSIGNED(prodXY_uid105_prod_uid47_fpMulTest_result_add_0_0_a) + UNSIGNED(prodXY_uid105_prod_uid47_fpMulTest_result_add_0_0_b));
    prodXY_uid105_prod_uid47_fpMulTest_result_add_0_0_q <= prodXY_uid105_prod_uid47_fpMulTest_result_add_0_0_o(48 downto 0);

    -- prodXY_uid105_prod_uid47_fpMulTest_result_add_1_0(ADD,125)@2
    prodXY_uid105_prod_uid47_fpMulTest_result_add_1_0_a <= STD_LOGIC_VECTOR("0" & prodXY_uid105_prod_uid47_fpMulTest_result_add_0_0_q);
    prodXY_uid105_prod_uid47_fpMulTest_result_add_1_0_b <= STD_LOGIC_VECTOR("00000000" & prodXY_uid105_prod_uid47_fpMulTest_align_15_q);
    prodXY_uid105_prod_uid47_fpMulTest_result_add_1_0_o <= STD_LOGIC_VECTOR(UNSIGNED(prodXY_uid105_prod_uid47_fpMulTest_result_add_1_0_a) + UNSIGNED(prodXY_uid105_prod_uid47_fpMulTest_result_add_1_0_b));
    prodXY_uid105_prod_uid47_fpMulTest_result_add_1_0_q <= prodXY_uid105_prod_uid47_fpMulTest_result_add_1_0_o(49 downto 0);

    -- osig_uid106_prod_uid47_fpMulTest(BITSELECT,105)@2
    osig_uid106_prod_uid47_fpMulTest_in <= prodXY_uid105_prod_uid47_fpMulTest_result_add_1_0_q(47 downto 0);
    osig_uid106_prod_uid47_fpMulTest_b <= osig_uid106_prod_uid47_fpMulTest_in(47 downto 0);

    -- normalizeBit_uid49_fpMulTest(BITSELECT,48)@2
    normalizeBit_uid49_fpMulTest_in <= STD_LOGIC_VECTOR(osig_uid106_prod_uid47_fpMulTest_b);
    normalizeBit_uid49_fpMulTest_b <= normalizeBit_uid49_fpMulTest_in(47 downto 47);

    -- roundBitDetectionConstant_uid63_fpMulTest(CONSTANT,62)
    roundBitDetectionConstant_uid63_fpMulTest_q <= "010";

    -- fracRPostNormHigh_uid51_fpMulTest(BITSELECT,50)@2
    fracRPostNormHigh_uid51_fpMulTest_in <= osig_uid106_prod_uid47_fpMulTest_b(46 downto 0);
    fracRPostNormHigh_uid51_fpMulTest_b <= fracRPostNormHigh_uid51_fpMulTest_in(46 downto 23);

    -- fracRPostNormLow_uid52_fpMulTest(BITSELECT,51)@2
    fracRPostNormLow_uid52_fpMulTest_in <= osig_uid106_prod_uid47_fpMulTest_b(45 downto 0);
    fracRPostNormLow_uid52_fpMulTest_b <= fracRPostNormLow_uid52_fpMulTest_in(45 downto 22);

    -- fracRPostNorm_uid53_fpMulTest(MUX,52)@2
    fracRPostNorm_uid53_fpMulTest_s <= normalizeBit_uid49_fpMulTest_b;
    fracRPostNorm_uid53_fpMulTest: PROCESS (fracRPostNorm_uid53_fpMulTest_s, en, fracRPostNormLow_uid52_fpMulTest_b, fracRPostNormHigh_uid51_fpMulTest_b)
    BEGIN
        CASE (fracRPostNorm_uid53_fpMulTest_s) IS
            WHEN "0" => fracRPostNorm_uid53_fpMulTest_q <= fracRPostNormLow_uid52_fpMulTest_b;
            WHEN "1" => fracRPostNorm_uid53_fpMulTest_q <= fracRPostNormHigh_uid51_fpMulTest_b;
            WHEN OTHERS => fracRPostNorm_uid53_fpMulTest_q <= (others => '0');
        END CASE;
    END PROCESS;

    -- fracRPostNorm1dto0_uid61_fpMulTest(BITSELECT,60)@2
    fracRPostNorm1dto0_uid61_fpMulTest_in <= fracRPostNorm_uid53_fpMulTest_q(1 downto 0);
    fracRPostNorm1dto0_uid61_fpMulTest_b <= fracRPostNorm1dto0_uid61_fpMulTest_in(1 downto 0);

    -- extraStickyBitOfProd_uid55_fpMulTest(BITSELECT,54)@2
    extraStickyBitOfProd_uid55_fpMulTest_in <= STD_LOGIC_VECTOR(osig_uid106_prod_uid47_fpMulTest_b(22 downto 0));
    extraStickyBitOfProd_uid55_fpMulTest_b <= extraStickyBitOfProd_uid55_fpMulTest_in(22 downto 22);

    -- extraStickyBit_uid56_fpMulTest(MUX,55)@2
    extraStickyBit_uid56_fpMulTest_s <= normalizeBit_uid49_fpMulTest_b;
    extraStickyBit_uid56_fpMulTest: PROCESS (extraStickyBit_uid56_fpMulTest_s, en, GND_q, extraStickyBitOfProd_uid55_fpMulTest_b)
    BEGIN
        CASE (extraStickyBit_uid56_fpMulTest_s) IS
            WHEN "0" => extraStickyBit_uid56_fpMulTest_q <= GND_q;
            WHEN "1" => extraStickyBit_uid56_fpMulTest_q <= extraStickyBitOfProd_uid55_fpMulTest_b;
            WHEN OTHERS => extraStickyBit_uid56_fpMulTest_q <= (others => '0');
        END CASE;
    END PROCESS;

    -- stickyRange_uid54_fpMulTest(BITSELECT,53)@2
    stickyRange_uid54_fpMulTest_in <= osig_uid106_prod_uid47_fpMulTest_b(21 downto 0);
    stickyRange_uid54_fpMulTest_b <= stickyRange_uid54_fpMulTest_in(21 downto 0);

    -- stickyExtendedRange_uid57_fpMulTest(BITJOIN,56)@2
    stickyExtendedRange_uid57_fpMulTest_q <= extraStickyBit_uid56_fpMulTest_q & stickyRange_uid54_fpMulTest_b;

    -- stickyRangeComparator_uid59_fpMulTest(LOGICAL,58)@2
    stickyRangeComparator_uid59_fpMulTest_a <= stickyExtendedRange_uid57_fpMulTest_q;
    stickyRangeComparator_uid59_fpMulTest_b <= cstZeroWF_uid11_fpMulTest_q;
    stickyRangeComparator_uid59_fpMulTest_q <= "1" WHEN stickyRangeComparator_uid59_fpMulTest_a = stickyRangeComparator_uid59_fpMulTest_b ELSE "0";

    -- sticky_uid60_fpMulTest(LOGICAL,59)@2
    sticky_uid60_fpMulTest_a <= stickyRangeComparator_uid59_fpMulTest_q;
    sticky_uid60_fpMulTest_q <= not (sticky_uid60_fpMulTest_a);

    -- lrs_uid62_fpMulTest(BITJOIN,61)@2
    lrs_uid62_fpMulTest_q <= fracRPostNorm1dto0_uid61_fpMulTest_b & sticky_uid60_fpMulTest_q;

    -- roundBitDetectionPattern_uid64_fpMulTest(LOGICAL,63)@2
    roundBitDetectionPattern_uid64_fpMulTest_a <= lrs_uid62_fpMulTest_q;
    roundBitDetectionPattern_uid64_fpMulTest_b <= roundBitDetectionConstant_uid63_fpMulTest_q;
    roundBitDetectionPattern_uid64_fpMulTest_q <= "1" WHEN roundBitDetectionPattern_uid64_fpMulTest_a = roundBitDetectionPattern_uid64_fpMulTest_b ELSE "0";

    -- roundBit_uid65_fpMulTest(LOGICAL,64)@2
    roundBit_uid65_fpMulTest_a <= roundBitDetectionPattern_uid64_fpMulTest_q;
    roundBit_uid65_fpMulTest_q <= not (roundBit_uid65_fpMulTest_a);

    -- roundBitAndNormalizationOp_uid68_fpMulTest(BITJOIN,67)@2
    roundBitAndNormalizationOp_uid68_fpMulTest_q <= GND_q & normalizeBit_uid49_fpMulTest_b & cstZeroWF_uid11_fpMulTest_q & roundBit_uid65_fpMulTest_q;

    -- biasInc_uid45_fpMulTest(CONSTANT,44)
    biasInc_uid45_fpMulTest_q <= "0001111111";

    -- expSum_uid44_fpMulTest(ADD,43)@0
    expSum_uid44_fpMulTest_a <= STD_LOGIC_VECTOR("0" & expX_uid6_fpMulTest_b);
    expSum_uid44_fpMulTest_b <= STD_LOGIC_VECTOR("0" & expY_uid7_fpMulTest_b);
    expSum_uid44_fpMulTest: PROCESS (clk, areset)
    BEGIN
        IF (areset = '1') THEN
            expSum_uid44_fpMulTest_o <= (others => '0');
        ELSIF (clk'EVENT AND clk = '1') THEN
            IF (en = "1") THEN
                expSum_uid44_fpMulTest_o <= STD_LOGIC_VECTOR(UNSIGNED(expSum_uid44_fpMulTest_a) + UNSIGNED(expSum_uid44_fpMulTest_b));
            END IF;
        END IF;
    END PROCESS;
    expSum_uid44_fpMulTest_q <= expSum_uid44_fpMulTest_o(8 downto 0);

    -- redist1(DELAY,127)
    redist1 : dspba_delay
    GENERIC MAP ( width => 9, depth => 1, reset_kind => "ASYNC" )
    PORT MAP ( xin => expSum_uid44_fpMulTest_q, xout => redist1_q, ena => en(0), clk => clk, aclr => areset );

    -- expSumMBias_uid46_fpMulTest(SUB,45)@2
    expSumMBias_uid46_fpMulTest_a <= STD_LOGIC_VECTOR(STD_LOGIC_VECTOR("0" & "00" & redist1_q));
    expSumMBias_uid46_fpMulTest_b <= STD_LOGIC_VECTOR(STD_LOGIC_VECTOR((11 downto 10 => biasInc_uid45_fpMulTest_q(9)) & biasInc_uid45_fpMulTest_q));
    expSumMBias_uid46_fpMulTest_o <= STD_LOGIC_VECTOR(SIGNED(expSumMBias_uid46_fpMulTest_a) - SIGNED(expSumMBias_uid46_fpMulTest_b));
    expSumMBias_uid46_fpMulTest_q <= expSumMBias_uid46_fpMulTest_o(10 downto 0);

    -- expFracPreRound_uid66_fpMulTest(BITJOIN,65)@2
    expFracPreRound_uid66_fpMulTest_q <= expSumMBias_uid46_fpMulTest_q & fracRPostNorm_uid53_fpMulTest_q;

    -- expFracRPostRounding_uid69_fpMulTest(ADD,68)@2
    expFracRPostRounding_uid69_fpMulTest_a <= STD_LOGIC_VECTOR(STD_LOGIC_VECTOR((36 downto 35 => expFracPreRound_uid66_fpMulTest_q(34)) & expFracPreRound_uid66_fpMulTest_q));
    expFracRPostRounding_uid69_fpMulTest_b <= STD_LOGIC_VECTOR(STD_LOGIC_VECTOR("0" & "0000000000" & roundBitAndNormalizationOp_uid68_fpMulTest_q));
    expFracRPostRounding_uid69_fpMulTest_o <= STD_LOGIC_VECTOR(SIGNED(expFracRPostRounding_uid69_fpMulTest_a) + SIGNED(expFracRPostRounding_uid69_fpMulTest_b));
    expFracRPostRounding_uid69_fpMulTest_q <= expFracRPostRounding_uid69_fpMulTest_o(35 downto 0);

    -- expRPreExcExt_uid71_fpMulTest(BITSELECT,70)@2
    expRPreExcExt_uid71_fpMulTest_in <= STD_LOGIC_VECTOR(expFracRPostRounding_uid69_fpMulTest_q);
    expRPreExcExt_uid71_fpMulTest_b <= expRPreExcExt_uid71_fpMulTest_in(35 downto 24);

    -- expRPreExc_uid72_fpMulTest(BITSELECT,71)@2
    expRPreExc_uid72_fpMulTest_in <= expRPreExcExt_uid71_fpMulTest_b(7 downto 0);
    expRPreExc_uid72_fpMulTest_b <= expRPreExc_uid72_fpMulTest_in(7 downto 0);

    -- expOvf_uid75_fpMulTest(COMPARE,74)@2
    expOvf_uid75_fpMulTest_cin <= GND_q;
    expOvf_uid75_fpMulTest_a <= STD_LOGIC_VECTOR(STD_LOGIC_VECTOR((13 downto 12 => expRPreExcExt_uid71_fpMulTest_b(11)) & expRPreExcExt_uid71_fpMulTest_b) & '0');
    expOvf_uid75_fpMulTest_b <= STD_LOGIC_VECTOR(STD_LOGIC_VECTOR("0" & "00000" & cstAllOWE_uid10_fpMulTest_q) & expOvf_uid75_fpMulTest_cin(0));
    expOvf_uid75_fpMulTest_o <= STD_LOGIC_VECTOR(SIGNED(expOvf_uid75_fpMulTest_a) - SIGNED(expOvf_uid75_fpMulTest_b));
    expOvf_uid75_fpMulTest_n(0) <= not (expOvf_uid75_fpMulTest_o(14));

    -- invExpXIsMax_uid35_fpMulTest(LOGICAL,34)@2
    invExpXIsMax_uid35_fpMulTest_a <= redist3_q;
    invExpXIsMax_uid35_fpMulTest_q <= not (invExpXIsMax_uid35_fpMulTest_a);

    -- InvExpXIsZero_uid36_fpMulTest(LOGICAL,35)@2
    InvExpXIsZero_uid36_fpMulTest_a <= redist4_q;
    InvExpXIsZero_uid36_fpMulTest_q <= not (InvExpXIsZero_uid36_fpMulTest_a);

    -- excR_y_uid37_fpMulTest(LOGICAL,36)@2
    excR_y_uid37_fpMulTest_a <= InvExpXIsZero_uid36_fpMulTest_q;
    excR_y_uid37_fpMulTest_b <= invExpXIsMax_uid35_fpMulTest_q;
    excR_y_uid37_fpMulTest_q <= excR_y_uid37_fpMulTest_a and excR_y_uid37_fpMulTest_b;

    -- invExpXIsMax_uid21_fpMulTest(LOGICAL,20)@2
    invExpXIsMax_uid21_fpMulTest_a <= redist6_q;
    invExpXIsMax_uid21_fpMulTest_q <= not (invExpXIsMax_uid21_fpMulTest_a);

    -- InvExpXIsZero_uid22_fpMulTest(LOGICAL,21)@2
    InvExpXIsZero_uid22_fpMulTest_a <= redist7_q;
    InvExpXIsZero_uid22_fpMulTest_q <= not (InvExpXIsZero_uid22_fpMulTest_a);

    -- excR_x_uid23_fpMulTest(LOGICAL,22)@2
    excR_x_uid23_fpMulTest_a <= InvExpXIsZero_uid22_fpMulTest_q;
    excR_x_uid23_fpMulTest_b <= invExpXIsMax_uid21_fpMulTest_q;
    excR_x_uid23_fpMulTest_q <= excR_x_uid23_fpMulTest_a and excR_x_uid23_fpMulTest_b;

    -- ExcROvfAndInReg_uid84_fpMulTest(LOGICAL,83)@2
    ExcROvfAndInReg_uid84_fpMulTest_a <= excR_x_uid23_fpMulTest_q;
    ExcROvfAndInReg_uid84_fpMulTest_b <= excR_y_uid37_fpMulTest_q;
    ExcROvfAndInReg_uid84_fpMulTest_c <= expOvf_uid75_fpMulTest_n;
    ExcROvfAndInReg_uid84_fpMulTest_q <= ExcROvfAndInReg_uid84_fpMulTest_a and ExcROvfAndInReg_uid84_fpMulTest_b and ExcROvfAndInReg_uid84_fpMulTest_c;

    -- excYRAndExcXI_uid83_fpMulTest(LOGICAL,82)@2
    excYRAndExcXI_uid83_fpMulTest_a <= excR_y_uid37_fpMulTest_q;
    excYRAndExcXI_uid83_fpMulTest_b <= excI_x_uid19_fpMulTest_q;
    excYRAndExcXI_uid83_fpMulTest_q <= excYRAndExcXI_uid83_fpMulTest_a and excYRAndExcXI_uid83_fpMulTest_b;

    -- excXRAndExcYI_uid82_fpMulTest(LOGICAL,81)@2
    excXRAndExcYI_uid82_fpMulTest_a <= excR_x_uid23_fpMulTest_q;
    excXRAndExcYI_uid82_fpMulTest_b <= excI_y_uid33_fpMulTest_q;
    excXRAndExcYI_uid82_fpMulTest_q <= excXRAndExcYI_uid82_fpMulTest_a and excXRAndExcYI_uid82_fpMulTest_b;

    -- excXIAndExcYI_uid81_fpMulTest(LOGICAL,80)@2
    excXIAndExcYI_uid81_fpMulTest_a <= excI_x_uid19_fpMulTest_q;
    excXIAndExcYI_uid81_fpMulTest_b <= excI_y_uid33_fpMulTest_q;
    excXIAndExcYI_uid81_fpMulTest_q <= excXIAndExcYI_uid81_fpMulTest_a and excXIAndExcYI_uid81_fpMulTest_b;

    -- excRInf_uid85_fpMulTest(LOGICAL,84)@2
    excRInf_uid85_fpMulTest_a <= excXIAndExcYI_uid81_fpMulTest_q;
    excRInf_uid85_fpMulTest_b <= excXRAndExcYI_uid82_fpMulTest_q;
    excRInf_uid85_fpMulTest_c <= excYRAndExcXI_uid83_fpMulTest_q;
    excRInf_uid85_fpMulTest_d <= ExcROvfAndInReg_uid84_fpMulTest_q;
    excRInf_uid85_fpMulTest_q <= excRInf_uid85_fpMulTest_a or excRInf_uid85_fpMulTest_b or excRInf_uid85_fpMulTest_c or excRInf_uid85_fpMulTest_d;

    -- expUdf_uid73_fpMulTest(COMPARE,72)@2
    expUdf_uid73_fpMulTest_cin <= GND_q;
    expUdf_uid73_fpMulTest_a <= STD_LOGIC_VECTOR(STD_LOGIC_VECTOR("0" & "000000000000" & GND_q) & '0');
    expUdf_uid73_fpMulTest_b <= STD_LOGIC_VECTOR(STD_LOGIC_VECTOR((13 downto 12 => expRPreExcExt_uid71_fpMulTest_b(11)) & expRPreExcExt_uid71_fpMulTest_b) & expUdf_uid73_fpMulTest_cin(0));
    expUdf_uid73_fpMulTest_o <= STD_LOGIC_VECTOR(SIGNED(expUdf_uid73_fpMulTest_a) - SIGNED(expUdf_uid73_fpMulTest_b));
    expUdf_uid73_fpMulTest_n(0) <= not (expUdf_uid73_fpMulTest_o(14));

    -- excZC3_uid79_fpMulTest(LOGICAL,78)@2
    excZC3_uid79_fpMulTest_a <= excR_x_uid23_fpMulTest_q;
    excZC3_uid79_fpMulTest_b <= excR_y_uid37_fpMulTest_q;
    excZC3_uid79_fpMulTest_c <= expUdf_uid73_fpMulTest_n;
    excZC3_uid79_fpMulTest_q <= excZC3_uid79_fpMulTest_a and excZC3_uid79_fpMulTest_b and excZC3_uid79_fpMulTest_c;

    -- excYZAndExcXR_uid78_fpMulTest(LOGICAL,77)@2
    excYZAndExcXR_uid78_fpMulTest_a <= redist4_q;
    excYZAndExcXR_uid78_fpMulTest_b <= excR_x_uid23_fpMulTest_q;
    excYZAndExcXR_uid78_fpMulTest_q <= excYZAndExcXR_uid78_fpMulTest_a and excYZAndExcXR_uid78_fpMulTest_b;

    -- excXZAndExcYR_uid77_fpMulTest(LOGICAL,76)@2
    excXZAndExcYR_uid77_fpMulTest_a <= redist7_q;
    excXZAndExcYR_uid77_fpMulTest_b <= excR_y_uid37_fpMulTest_q;
    excXZAndExcYR_uid77_fpMulTest_q <= excXZAndExcYR_uid77_fpMulTest_a and excXZAndExcYR_uid77_fpMulTest_b;

    -- excXZAndExcYZ_uid76_fpMulTest(LOGICAL,75)@2
    excXZAndExcYZ_uid76_fpMulTest_a <= redist7_q;
    excXZAndExcYZ_uid76_fpMulTest_b <= redist4_q;
    excXZAndExcYZ_uid76_fpMulTest_q <= excXZAndExcYZ_uid76_fpMulTest_a and excXZAndExcYZ_uid76_fpMulTest_b;

    -- excRZero_uid80_fpMulTest(LOGICAL,79)@2
    excRZero_uid80_fpMulTest_a <= excXZAndExcYZ_uid76_fpMulTest_q;
    excRZero_uid80_fpMulTest_b <= excXZAndExcYR_uid77_fpMulTest_q;
    excRZero_uid80_fpMulTest_c <= excYZAndExcXR_uid78_fpMulTest_q;
    excRZero_uid80_fpMulTest_d <= excZC3_uid79_fpMulTest_q;
    excRZero_uid80_fpMulTest_q <= excRZero_uid80_fpMulTest_a or excRZero_uid80_fpMulTest_b or excRZero_uid80_fpMulTest_c or excRZero_uid80_fpMulTest_d;

    -- concExc_uid90_fpMulTest(BITJOIN,89)@2
    concExc_uid90_fpMulTest_q <= excRNaN_uid89_fpMulTest_q & excRInf_uid85_fpMulTest_q & excRZero_uid80_fpMulTest_q;

    -- excREnc_uid91_fpMulTest(LOOKUP,90)@2
    excREnc_uid91_fpMulTest: PROCESS (concExc_uid90_fpMulTest_q)
    BEGIN
        -- Begin reserved scope level
        CASE (concExc_uid90_fpMulTest_q) IS
            WHEN "000" => excREnc_uid91_fpMulTest_q <= "01";
            WHEN "001" => excREnc_uid91_fpMulTest_q <= "00";
            WHEN "010" => excREnc_uid91_fpMulTest_q <= "10";
            WHEN "011" => excREnc_uid91_fpMulTest_q <= "00";
            WHEN "100" => excREnc_uid91_fpMulTest_q <= "11";
            WHEN "101" => excREnc_uid91_fpMulTest_q <= "00";
            WHEN "110" => excREnc_uid91_fpMulTest_q <= "00";
            WHEN "111" => excREnc_uid91_fpMulTest_q <= "00";
            WHEN OTHERS => -- unreachable
                           excREnc_uid91_fpMulTest_q <= (others => '-');
        END CASE;
        -- End reserved scope level
    END PROCESS;

    -- expRPostExc_uid100_fpMulTest(MUX,99)@2
    expRPostExc_uid100_fpMulTest_s <= excREnc_uid91_fpMulTest_q;
    expRPostExc_uid100_fpMulTest: PROCESS (expRPostExc_uid100_fpMulTest_s, en, cstAllZWE_uid12_fpMulTest_q, expRPreExc_uid72_fpMulTest_b, cstAllOWE_uid10_fpMulTest_q)
    BEGIN
        CASE (expRPostExc_uid100_fpMulTest_s) IS
            WHEN "00" => expRPostExc_uid100_fpMulTest_q <= cstAllZWE_uid12_fpMulTest_q;
            WHEN "01" => expRPostExc_uid100_fpMulTest_q <= expRPreExc_uid72_fpMulTest_b;
            WHEN "10" => expRPostExc_uid100_fpMulTest_q <= cstAllOWE_uid10_fpMulTest_q;
            WHEN "11" => expRPostExc_uid100_fpMulTest_q <= cstAllOWE_uid10_fpMulTest_q;
            WHEN OTHERS => expRPostExc_uid100_fpMulTest_q <= (others => '0');
        END CASE;
    END PROCESS;

    -- oneFracRPostExc2_uid92_fpMulTest(CONSTANT,91)
    oneFracRPostExc2_uid92_fpMulTest_q <= "00000000000000000000001";

    -- fracRPreExc_uid70_fpMulTest(BITSELECT,69)@2
    fracRPreExc_uid70_fpMulTest_in <= expFracRPostRounding_uid69_fpMulTest_q(23 downto 0);
    fracRPreExc_uid70_fpMulTest_b <= fracRPreExc_uid70_fpMulTest_in(23 downto 1);

    -- fracRPostExc_uid95_fpMulTest(MUX,94)@2
    fracRPostExc_uid95_fpMulTest_s <= excREnc_uid91_fpMulTest_q;
    fracRPostExc_uid95_fpMulTest: PROCESS (fracRPostExc_uid95_fpMulTest_s, en, cstZeroWF_uid11_fpMulTest_q, fracRPreExc_uid70_fpMulTest_b, oneFracRPostExc2_uid92_fpMulTest_q)
    BEGIN
        CASE (fracRPostExc_uid95_fpMulTest_s) IS
            WHEN "00" => fracRPostExc_uid95_fpMulTest_q <= cstZeroWF_uid11_fpMulTest_q;
            WHEN "01" => fracRPostExc_uid95_fpMulTest_q <= fracRPreExc_uid70_fpMulTest_b;
            WHEN "10" => fracRPostExc_uid95_fpMulTest_q <= cstZeroWF_uid11_fpMulTest_q;
            WHEN "11" => fracRPostExc_uid95_fpMulTest_q <= oneFracRPostExc2_uid92_fpMulTest_q;
            WHEN OTHERS => fracRPostExc_uid95_fpMulTest_q <= (others => '0');
        END CASE;
    END PROCESS;

    -- R_uid103_fpMulTest(BITJOIN,102)@2
    R_uid103_fpMulTest_q <= signRPostExc_uid102_fpMulTest_q & expRPostExc_uid100_fpMulTest_q & fracRPostExc_uid95_fpMulTest_q;

    -- xOut(GPOUT,4)@2
    q <= R_uid103_fpMulTest_q;

END normal;
