-------------------------------------------------------------------------------
-- Title      : Clock Generator
-- Project    : HFT PXL
-------------------------------------------------------------------------------
-- File       : Clock_generator.vhd
-- Author     : Joachim Schambach
-- Company    : University of Texas 
-- Created    : 2012-02-16
-- Last update: 2013-12-12
-- Platform   : Windows, Xilinx PlanAhead/ISE 14.5
-- Target     : Virtex-6 (XC6VLX240T-FF1759)
-- Standard   : VHDL'93/02
-------------------------------------------------------------------------------
-- Description: Generate synthesized clocks needed for Virtex 6 board
--              Uses MMCM_ADV primitive, and global clock buffers for ALL
--              generated clocks.
--              Info from the Core Clock Generator Wizard:
------------------------------------------------------------------------------
--      "Output    Output      Phase     Duty      Pk-to-Pk        Phase"
--      "Clock    Freq (MHz) (degrees) Cycle (%) Jitter (ps)  Error (ps)"
------------------------------------------------------------------------------
--      CLK_OUT1___200.000______0.000______50.0______161.087____144.334
--      CLK_OUT2___160.000______0.000______50.0______167.571____144.334
--      CLK_OUT3___100.000______0.000______50.0______186.330____144.334
--      CLK_OUT4____20.000______0.000______50.0______270.105____144.334
--      CLK_OUT5____10.000______0.000______50.0______309.743____144.334
--      CLK_BUFG____50.000______0.000______50.0______222.587____144.334
--
------------------------------------------------------------------------------
--      "Input Clock   Freq (MHz)    Input Jitter (UI)"
------------------------------------------------------------------------------
--      __primary__________50.000____________0.010

-------------------------------------------------------------------------------
-- Copyright (c) 2012 
-------------------------------------------------------------------------------
-- Revisions  :
-- Date        Version  Author          Description
-- 2012-02-16  1.0      jschamba        Created
-------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

LIBRARY unisim;
USE unisim.vcomponents.ALL;

ENTITY Clock_generator IS
  PORT (
    -- Clock in ports
    CLK_IN1   : IN  std_logic;
    -- Clock out ports
    CLK_OUT1  : OUT std_logic;
    CLK_OUT2  : OUT std_logic;
    CLK_OUT2B : OUT std_logic;
    CLK_OUT3  : OUT std_logic;
    CLK_OUT4  : OUT std_logic;
    CLK_OUT5  : OUT std_logic;
    CLK_BUFG  : OUT std_logic;
    CLK_BUFGB : OUT std_logic;
    -- Status and control signals
    RESET     : IN  std_logic;
    LOCKED    : OUT std_logic
    );
END Clock_generator;

ARCHITECTURE xilinx OF Clock_generator IS
  ATTRIBUTE CORE_GENERATION_INFO           : string;
  ATTRIBUTE CORE_GENERATION_INFO OF xilinx : ARCHITECTURE IS "Clock_generator,clk_wiz_v3_6,{component_name=Clock_generator,use_phase_alignment=false,use_min_o_jitter=false,use_max_i_jitter=false,use_dyn_phase_shift=false,use_inclk_switchover=false,use_dyn_reconfig=false,feedback_source=FDBK_AUTO,primtype_sel=MMCM_ADV,num_out_clk=6,clkin1_period=20.000,clkin2_period=10.0,use_power_down=false,use_reset=true,use_locked=true,use_inclk_stopped=false,use_status=false,use_freeze=false,use_clk_valid=false,feedback_type=SINGLE,clock_mgr_type=MANUAL,manual_override=false}";

  -- Input clock buffering / unused connectors
  SIGNAL clkin1   : std_logic;
  -- Output clock buffering / unused connectors
  SIGNAL clkfbout : std_logic;
  SIGNAL clkout0  : std_logic;
  SIGNAL clkout0b : std_logic;
  SIGNAL clkout1  : std_logic;
  SIGNAL clkout1b : std_logic;
  SIGNAL clkout2  : std_logic;
  SIGNAL clkout3  : std_logic;
  SIGNAL clkout4  : std_logic;
  SIGNAL clkout5  : std_logic;

BEGIN

  -- Input buffering
  --------------------------------------
  clkin1_buf : IBUFG
    PORT MAP (
      O => clkin1,
      I => CLK_IN1
      );


  -- Clocking primitive
  --------------------------------------
  -- Instantiation of the MMCM primitive
  mmcm_adv_inst : MMCM_ADV
    GENERIC MAP (
      BANDWIDTH            => "OPTIMIZED",
      CLKOUT4_CASCADE      => false,
      CLOCK_HOLD           => false,
      COMPENSATION         => "ZHOLD",
      STARTUP_WAIT         => false,
      DIVCLK_DIVIDE        => 1,
      CLKFBOUT_MULT_F      => 16.000,
      CLKFBOUT_PHASE       => 0.000,
      CLKFBOUT_USE_FINE_PS => false,
      -- 50 MHz
      CLKOUT0_DIVIDE_F     => 16.000,
      CLKOUT0_PHASE        => 0.000,
      CLKOUT0_DUTY_CYCLE   => 0.500,
      CLKOUT0_USE_FINE_PS  => false,
      -- 160 MHz
      CLKOUT1_DIVIDE       => 5,
      CLKOUT1_PHASE        => 0.000,
      CLKOUT1_DUTY_CYCLE   => 0.500,
      CLKOUT1_USE_FINE_PS  => false,
      -- 100 MHz
      CLKOUT2_DIVIDE       => 8,
      CLKOUT2_PHASE        => 0.000,
      CLKOUT2_DUTY_CYCLE   => 0.500,
      CLKOUT2_USE_FINE_PS  => false,
      -- 25 MHz
      CLKOUT3_DIVIDE       => 32,
      CLKOUT3_PHASE        => 0.000,
      CLKOUT3_DUTY_CYCLE   => 0.500,
      CLKOUT3_USE_FINE_PS  => false,
      -- 12.5 MHz
      CLKOUT4_DIVIDE       => 64,
      CLKOUT4_PHASE        => 0.000,
      CLKOUT4_DUTY_CYCLE   => 0.500,
      CLKOUT4_USE_FINE_PS  => false,
      -- 200 MHz
      CLKOUT5_DIVIDE       => 4,
      CLKOUT5_PHASE        => 0.000,
      CLKOUT5_DUTY_CYCLE   => 0.500,
      CLKOUT5_USE_FINE_PS  => false,
      -- input clock: 50 MHz
      CLKIN1_PERIOD        => 20.000,
      REF_JITTER1          => 0.010
      )
    PORT MAP (
      -- output clocks
      CLKFBOUT     => clkfbout,
      CLKFBOUTB    => OPEN,
      CLKOUT0      => clkout0,
      CLKOUT0B     => clkout0b,
      CLKOUT1      => clkout1,
      CLKOUT1B     => clkout1b,
      CLKOUT2      => clkout2,
      CLKOUT2B     => OPEN,
      CLKOUT3      => clkout3,
      CLKOUT3B     => OPEN,
      CLKOUT4      => clkout4,
      CLKOUT5      => clkout5,
      CLKOUT6      => OPEN,
      -- Input clock control
      CLKFBIN      => clkfbout,
      CLKIN1       => clkin1,
      CLKIN2       => '0',
      -- Tied to always select the primary input clock
      CLKINSEL     => '1',
      -- Ports for dynamic reconfiguration
      DADDR        => (OTHERS => '0'),
      DCLK         => '0',
      DEN          => '0',
      DI           => (OTHERS => '0'),
      DO           => OPEN,
      DRDY         => OPEN,
      DWE          => '0',
      -- Ports for dynamic phase shift
      PSCLK        => '0',
      PSEN         => '0',
      PSINCDEC     => '0',
      PSDONE       => OPEN,
      -- Other control and status signals
      LOCKED       => LOCKED,
      CLKINSTOPPED => OPEN,
      CLKFBSTOPPED => OPEN,
      PWRDWN       => '0',
      RST          => RESET
      );

  -- Output buffering
  -------------------------------------
  clkout1_buf : BUFG
    PORT MAP (
      O => CLK_OUT1,
      I => clkout5
      );

  clkout2_buf : BUFG
    PORT MAP (
      O => CLK_OUT2,
      I => clkout1
      );

  clkout2b_buf : BUFG
    PORT MAP (
      O => CLK_OUT2B,
      I => clkout1b
      );

  clkout3_buf : BUFG
    PORT MAP (
      O => CLK_OUT3,
      I => clkout2
      );

  clkout4_buf : BUFG
    PORT MAP (
      O => CLK_OUT4,
      I => clkout3
      );

  clkout5_buf : BUFG
    PORT MAP (
      O => CLK_OUT5,
      I => clkout4
      );

  clkoutbufg_buf : BUFG
    PORT MAP (
      O => CLK_BUFG,
      I => clkout0
      );

  clkoutbufgb_buf : BUFG
    PORT MAP (
      O => CLK_BUFGB,
      I => clkout0b
      );

END xilinx;
