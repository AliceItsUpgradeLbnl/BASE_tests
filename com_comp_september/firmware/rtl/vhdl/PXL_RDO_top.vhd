-------------------------------------------------------------------------------
-- Title      : HFT PXL RDO board Top Module
-- Project    : HFT PXL - modified version for BASE tests
-------------------------------------------------------------------------------
-- File       : PXL_RDO_top.vhd
-- Author     : Joachim Schambach (jschamba@physics.utexas.edu)
-- Company    : University of Texas at Austin
-- Created    : 2012-02-16
-- Last update: 2016-03-16 (Alberto Collu)
-- Platform   : Linux
-- Target     : Virtex-6 (XC6VLX240T-FF1759)
-- Standard   : VHDL / verilog
-------------------------------------------------------------------------------
-- Description: Top Level design for Virtex-6 RDO board
-------------------------------------------------------------------------------
-- Copyright (c) 2012 
-------------------------------------------------------------------------------
-- Revisions  :
-- Date        Version  Author          Description
-- 2012-02-16  1.0      jschamba        Created
-- 2016-03-16  2.0      acollu          Modified (modifications for rad tests)
-------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

LIBRARY UNISIM;
USE UNISIM.VComponents.ALL;

LIBRARY work;
USE work.utilities.ALL;

ENTITY PXL_RDO_top IS
  GENERIC (
    -- this generic controls the inclusion of the chipscope code
    -- at the bottom of this file:
    bIncludeChipscope : boolean := true);
  PORT (
    ---------------------------------------------------------------------------
    -- Board clock
    ---------------------------------------------------------------------------

    OSC_50MHZ         : IN    std_logic;

    ---------------------------------------------------------------------------
    -- USB interface
    ---------------------------------------------------------------------------    

    MFT_RESET_n       : OUT   std_logic;

    MFTA_D            : INOUT std_logic_vector (7 DOWNTO 0);
    MFTB_D            : INOUT std_logic_vector (7 DOWNTO 0);
    -- 
    MFTA_RXF_n        : IN    std_logic;
    MFTA_TXE_n        : IN    std_logic;
    MFTA_RD_n         : OUT   std_logic;
    MFTA_WR_n         : OUT   std_logic;
    MFTA_SIWU         : OUT   std_logic;
    MFTA_CLKOUT       : IN    std_logic;
    MFTA_OE_n         : OUT   std_logic;

    MFTB_RXF_n        : IN    std_logic;
    MFTB_RD_n         : OUT   std_logic;
    MFTB_WR_n         : OUT   std_logic;
    MFTB_SIWU         : OUT   std_logic;
    MFTB_OE_n         : OUT   std_logic;

    ---------------------------------------------------------------------------
    -- Configuration pins for daughter card connections
    ---------------------------------------------------------------------------

    DC_NOT_ADD_EN_BAR : OUT   std_logic;
    FLASH_ADD_EN_BAR  : OUT   std_logic;
	  
    DC_A              : INOUT std_logic_vector(100 downto 1);
    DC_A_N            : INOUT std_logic_vector(20 downto 1);
    DC_A_P            : INOUT std_logic_vector(20 downto 1);
    DC_B              : INOUT std_logic_vector(100 downto 1)

    );
END PXL_RDO_top;

-------------------------------------------------------------------------------
-- Begin of the Architecture 
-------------------------------------------------------------------------------

ARCHITECTURE behavior OF PXL_RDO_top IS

  -----------------------------------------------------------------------------
  -- Constants

  CONSTANT HEADER_PATTERN  : std_logic_vector(15 DOWNTO 0) := x"AAAA";
  CONSTANT TRAILER_PATTERN : std_logic_vector(15 DOWNTO 0) := x"5678";

  -----------------------------------------------------------------------------
  -- Components

  COMPONENT Clock_generator
    PORT (
      CLK_IN1   : IN  std_logic;
      CLK_OUT1  : OUT std_logic;
      CLK_OUT2  : OUT std_logic;
      CLK_OUT2B : OUT std_logic;
      CLK_OUT3  : OUT std_logic;
      CLK_OUT4  : OUT std_logic;
      CLK_OUT5  : OUT std_logic;
      CLK_BUFG  : OUT std_logic;        -- input clock buffered
      CLK_BUFGB : OUT std_logic;        -- input clock buffered inverted
      RESET     : IN  std_logic;
      LOCKED    : OUT std_logic
      );
  END COMPONENT;

  COMPONENT control_interface IS
    PORT (
      RESET           : IN  std_logic;
      CLK             : IN  std_logic;
      FIFO_Q          : OUT std_logic_vector(35 DOWNTO 0);
      FIFO_EMPTY      : OUT std_logic;
      FIFO_RDREQ      : IN  std_logic;
      FIFO_RDCLK      : IN  std_logic;
      CMD_FIFO_Q      : IN  std_logic_vector(35 DOWNTO 0);
      CMD_FIFO_EMPTY  : IN  std_logic;
      CMD_FIFO_RDREQ  : OUT std_logic;
      BUSY_RT         : IN std_logic; 
      FIFO_DATA       : IN std_logic_vector(31 DOWNTO 0);   
      FIFO_WREN       : IN std_logic;    
      FIFO_FULL       : OUT std_logic;
      REG_DATA        : IN std_logic_vector(15 DOWNTO 0);
      REG_WREN        : OUT std_logic   
      );
  END COMPONENT;

  COMPONENT M_FT2232H IS
    PORT (
      CLK            : IN  std_logic;
      RESET          : IN  std_logic;
      D_in           : IN  std_logic_vector (7 DOWNTO 0);
      D_out          : OUT std_logic_vector (7 DOWNTO 0);
      D_T            : OUT std_logic;
      RXF_n          : IN  std_logic;
      TXE_n          : IN  std_logic;
      RD_n           : OUT std_logic;
      WR_n           : OUT std_logic;
      SIWU           : OUT std_logic;
      CLKOUT         : IN  std_logic;
      OE_n           : OUT std_logic;
      FIFO_Q         : IN  std_logic_vector(35 DOWNTO 0);
      FIFO_EMPTY     : IN  std_logic;
      FIFO_RDREQ     : OUT std_logic;
      FIFO_RDCLK     : OUT std_logic;
      CMD_FIFO_Q     : OUT std_logic_vector(35 DOWNTO 0);
      CMD_FIFO_EMPTY : OUT std_logic;
      CMD_FIFO_RDREQ : IN  std_logic
      );
  END COMPONENT M_FT2232H;

  COMPONENT rad_test_int IS
    GENERIC (
      INPUT_CLK_FREQUENCY : integer := 50000000;   
      I2C_CLK_FREQUENCY : integer := 50000;    
      SPI_CLK_FREQUENCY : integer := 25000000     
    );
    PORT (
      CLK           : IN std_logic;
      FIFO_FULL     : IN std_logic;
      REG_ADDR      : IN std_logic_vector(3 DOWNTO 0);
      REG_IN        : IN std_logic_vector(15 DOWNTO 0);
      REG_WREN      : IN std_logic;                                            
      RST_N         : IN std_logic;
      SPI_CLK       : IN std_logic;
      BUSY          : OUT std_logic;
      FIFO_DATA_IN  : OUT std_logic_vector(31 DOWNTO 0);   
      FIFO_WREN     : OUT std_logic;    
      REG_OUT       : OUT std_logic_vector(15 DOWNTO 0);    
      DC_A          : INOUT std_logic_vector(100 DOWNTO 1);
      DC_A_N        : INOUT std_logic_vector(20 DOWNTO 1);
      DC_A_P        : INOUT std_logic_vector(20 DOWNTO 1);
      DC_B          : INOUT std_logic_vector(100 DOWNTO 1)
    );
  END COMPONENT rad_test_int;

  ---------------------------------------------< Reset signals

  SIGNAL sClkRST     : std_logic;
  SIGNAL sGlobalRST  : std_logic;
  SIGNAL sDelayRST   : std_logic;
  SIGNAL sDelayRST_n : std_logic;

  ---------------------------------------------> Reset signals

  ---------------------------------------------< MB FTDI signals

  SIGNAL sMFTA_rst          : std_logic;
  SIGNAL sMFTA_D_in         : std_logic_vector (7 DOWNTO 0);
  SIGNAL sMFTA_D_out        : std_logic_vector (7 DOWNTO 0);
  SIGNAL sMFTA_D_T          : std_logic;
  SIGNAL sMFTA_FifoQ        : std_logic_vector(35 DOWNTO 0);
  SIGNAL sMFTA_FifoEmpty    : std_logic;
  SIGNAL sMFTA_FifoRdreq    : std_logic;
  SIGNAL sMFTA_FifoRdClk    : std_logic;
  SIGNAL sMFTA_CmdFifoQ     : std_logic_vector(35 DOWNTO 0);
  SIGNAL sMFTA_CmdFifoEmpty : std_logic;
  SIGNAL sMFTA_CmdFifoRdreq : std_logic;
  SIGNAL sMFTB_D_in         : std_logic_vector (7 DOWNTO 0);
  SIGNAL sMFTB_D_out        : std_logic_vector (7 DOWNTO 0);
  SIGNAL sMFTB_D_T          : std_logic;

  SIGNAL sMFTA_RD_n : std_logic;
  SIGNAL sMFTA_WR_n : std_logic;
  SIGNAL sMFTA_SIWU : std_logic;

  SIGNAL sMftb_reset : std_logic;
  TYPE mftbState_type IS (MFTB0, MFTB1, MFTB2, MFTB3);
  SIGNAL mftbState   : mftbState_type;

  ---------------------------------------------> MB FTDI signals

  ---------------------------------------------< ControlIF signals

  SIGNAL sStatusReg     : std_logic_vector (255 DOWNTO 0);
  SIGNAL sMemWe         : std_logic;
  SIGNAL sMemAddr       : std_logic_vector(31 DOWNTO 0);
  SIGNAL sMemDin        : std_logic_vector(31 DOWNTO 0);
  SIGNAL sConfigReg     : std_logic_vector(511 DOWNTO 0);
  SIGNAL sCtl_FifoRdreq : std_logic;
  SIGNAL sPulseReg      : std_logic_vector(15 DOWNTO 0);

  ---------------------------------------------> ControlIF signals

  ---------------------------------------------< SIU signals

  SIGNAL sSiu_rst          : std_logic;
  SIGNAL sSiu_FeeReset     : std_logic;
  SIGNAL sEvtRdEnable      : std_logic;
  SIGNAL sSiu_T            : std_logic;
  SIGNAL sSiu_fiTEN_n      : std_logic;
  SIGNAL sSiu_foTEN_n      : std_logic;
  SIGNAL sSiu_fiCTRL_n     : std_logic;
  SIGNAL sSiu_foCTRL_n     : std_logic;
  SIGNAL sSiu_fiD          : std_logic_vector (31 DOWNTO 0);
  SIGNAL sSiu_foD          : std_logic_vector (31 DOWNTO 0);
  SIGNAL sSiu_FifoEmpty    : std_logic;
  SIGNAL sSiu_FifoRdreq    : std_logic;
  SIGNAL sSiu_CmdFifoQ     : std_logic_vector(35 DOWNTO 0);
  SIGNAL sSiu_CmdFifoEmpty : std_logic;
  SIGNAL sSiu_CmdFifoRdreq : std_logic;

  ---------------------------------------------> SIU signals

  ---------------------------------------------< Clock signals

  SIGNAL sClk200    : std_logic;
  SIGNAL sClk160    : std_logic;
  SIGNAL sClk160b   : std_logic;
  SIGNAL sClk100    : std_logic;
  SIGNAL sClk50     : std_logic;
  SIGNAL sClk50b    : std_logic;
  SIGNAL sClk25     : std_logic;
  SIGNAL sClk12p5   : std_logic;
  SIGNAL sClkLocked : std_logic;

  ---------------------------------------------> Clock signals

  ---------------------------------------------< Expansion board signals

  SIGNAL sDcSDA_in  : std_logic;
  SIGNAL sDcSDA_out : std_logic;
  SIGNAL sDcSCL     : std_logic;

  ---------------------------------------------> Expansion board signals

  ---------------------------------------------< D.C. ADS5270 signals

  SIGNAL sAdc_out      : std_logic_vector (8 DOWNTO 1);
  SIGNAL sAdc_frameclk : std_logic;
  SIGNAL sAdc_lclk     : std_logic;
  SIGNAL sAdc_cs_n     : std_logic;
  SIGNAL sAdc_sclk     : std_logic;
  SIGNAL sAdc_sda      : std_logic;
  SIGNAL sAdc_reset_n  : std_logic;
  SIGNAL sAdc_adclk    : std_logic;

  ---------------------------------------------> D.C. ADS5270 signals

  ---------------------------------------------< MTB I2C signals

  SIGNAL sI2C_sda_in    : std_logic;
  SIGNAL sI2C_sda_out   : std_logic;
  SIGNAL sI2C_sda_T     : std_logic;
  SIGNAL sI2C_addr      : std_logic_vector(6 DOWNTO 0);
  SIGNAL sI2C_rw        : std_logic;
  SIGNAL sI2C_busy      : std_logic;
  SIGNAL sI2C_ack_error : std_logic;
  SIGNAL sI2C_data      : std_logic_vector(31 DOWNTO 0);
  SIGNAL sN_CONVST      : std_logic;
  SIGNAL sSCL           : std_logic;

  ---------------------------------------------> MTB I2C signals


  ---------------------------------------------< Sensor signals
  SIGNAL sChipStart      : std_logic_vector (4 DOWNTO 1);
  SIGNAL iStartCtr       : integer RANGE 0 TO 127 := 0;
  SIGNAL sMarkerL        : std_logic_vector (4 DOWNTO 1);
  SIGNAL sSensorOut      : std_logic_vector (79 DOWNTO 0);
  SIGNAL sSensorDelayed  : std_logic_vector (79 DOWNTO 0);
  SIGNAL sLUanalog_T     : std_logic_vector (4 DOWNTO 1);
  SIGNAL sLUdigital_T    : std_logic_vector (4 DOWNTO 1);
  SIGNAL sLUanalog_in    : std_logic_vector (4 DOWNTO 1);
  SIGNAL sLUdigital_in   : std_logic_vector (4 DOWNTO 1);
  SIGNAL sLUanalog_out   : std_logic_vector (4 DOWNTO 1);
  SIGNAL sLUdigital_out  : std_logic_vector (4 DOWNTO 1);
  SIGNAL sLUmtb_T        : std_logic;
  SIGNAL sLUmtb_in       : std_logic;
  SIGNAL sLUmtb_out      : std_logic;
  SIGNAL sLClkEnable_n   : std_logic;
  SIGNAL sMtbPllEnable_n : std_logic;
  SIGNAL sChSt_trigger   : std_logic;

  TYPE chStState_type IS (CS0, CS1);
  SIGNAL chStState : chStState_type;
  ---------------------------------------------> Sensor signals

  ---------------------------------------------< Latchup signals
  SIGNAL sLuStart       : std_logic;
  SIGNAL sLuJtagStart   : std_logic_vector(4 DOWNTO 1);
  SIGNAL sLuChipStart   : std_logic;
  SIGNAL sLU_in         : std_logic_vector(8 DOWNTO 0);
  SIGNAL sLU_out        : std_logic_vector(8 DOWNTO 0);
  SIGNAL sLU_T          : std_logic_vector(8 DOWNTO 0);
  SIGNAL sLuJtagRst     : std_logic;
  SIGNAL sLuBusy        : std_logic;
  SIGNAL sLuWhichLadder : std_logic_vector(4 DOWNTO 1);
  SIGNAL sLuWhichLU     : std_logic_vector(2 DOWNTO 0);
  SIGNAL sLuCtr         : std_logic_vector(71 DOWNTO 0);
  SIGNAL sLuCtrRst      : std_logic;
  SIGNAL sLuAutoReset   : std_logic;
  SIGNAL sLuClkEnable   : std_logic;
  ---------------------------------------------> Latchup signals

  ---------------------------------------------< Radiation tests block

  SIGNAL sFIFOFullRT    : std_logic;
  SIGNAL sRegWrEnRT     : std_logic;                                            
  SIGNAL sBusyRT        : std_logic; 
  SIGNAL sFIFODataRT    : std_logic_vector(31 DOWNTO 0);   
  SIGNAL sFIFOWrEnRT    : std_logic;    
  SIGNAL SRegDataRT     : std_logic_vector(15 DOWNTO 0);

  ---------------------------------------------> Radiation tests block

  ---------------------------------------------< IODELAY signals
  SIGNAL sIdelayctrl_RDY : std_logic;
  SIGNAL sIodelayRst     : std_logic;
  ---------------------------------------------> IODELAY signals

  ---------------------------------------------< Chipscope signals
  SIGNAL sCS_control0 : std_logic_vector (35 DOWNTO 0);
  SIGNAL sCS_trig0    : std_logic_vector (170 DOWNTO 0);
  ---------------------------------------------> Chipscope signals


-------------------------------------------------------------------------------
-- Start of the implementation
-------------------------------------------------------------------------------

BEGIN
  -- chip enable signals for the dual purpose pins
  -- that also service the flash address lines
  DC_NOT_ADD_EN_BAR <= '0';             -- use as I/O
  FLASH_ADD_EN_BAR  <= '1';             -- not Flash Address

---------------------------------------------< Clocks

  -- global reset while MMCM is not locked
  sClkRST    <= '0';
  sGlobalRST <= (NOT sClkLocked) OR sMftb_reset;

-------------------------------------------------------------------------------
-- Generate all the clocks needed for the design
-- Use global buffers for all clock lines
-------------------------------------------------------------------------------

  -- MMCM component instance
  clockg_inst : Clock_generator
    PORT MAP (
      CLK_IN1   => OSC_50MHZ,
      CLK_OUT1  => sClk200,             -- 200 MHz
      CLK_OUT2  => sClk160,             -- 160 MHz
      CLK_OUT2B => sClk160b,            -- 160 MHz inverted
      CLK_OUT3  => sClk100,             -- 100 MHz
      CLK_OUT4  => sClk25,              --  25 MHz (20 MHz previously)
      CLK_OUT5  => sClk12p5,            --  12.5 MHz
      CLK_BUFG  => sClk50,              -- original 50 MHz, buffered
      CLK_BUFGB => sClk50b,             -- original 50 MHz, inverted
      RESET     => sClkRST,
      LOCKED    => sClkLocked
      );

---------------------------------------------> Clocks

---------------------------------------------< MB FTDI

  -- bi-directional data signals:
  mfta_bufd : FOR i IN 0 TO 7 GENERATE  -- A port
    IOBUF_mfta_di : IOBUF
      PORT MAP (
        O  => sMFTA_D_in(i),            -- Buffer output
        IO => MFTA_D(i),  -- Buffer inout port (connect directly to top-level port)
        I  => sMFTA_D_out(i),           -- Buffer input
        T  => sMFTA_D_T   -- 3-state enable signal, 1=input, 0=output
        );
  END GENERATE mfta_bufd;

  mftb_bufd : FOR i IN 0 TO 7 GENERATE  -- B port
    IOBUF_mftb_di : IOBUF
      PORT MAP (
        O  => sMFTB_D_in(i),            -- Buffer output
        IO => MFTB_D(i),  -- Buffer inout port (connect directly to top-level port)
        I  => sMFTB_D_out(i),           -- Buffer input
        T  => sMFTB_D_T   -- 3-state enable signal, 1=input, 0=output
        );
  END GENERATE mftb_bufd;

  MFT_RESET_n <= '1';

  MFTA_RD_n <= sMFTA_RD_n;
  MFTA_WR_n <= sMFTA_WR_n;
  MFTA_SIWU <= sMFTA_SIWU;

  sMFTA_rst <= sGlobalRst;
  mft2232h_instA : M_FT2232H
    PORT MAP (
      CLK            => sClk50,
      RESET          => sMFTA_rst,
      D_in           => sMFTA_D_in,
      D_out          => sMFTA_D_out,
      D_T            => sMFTA_D_T,
      RXF_n          => MFTA_RXF_n,
      TXE_n          => MFTA_TXE_n,
      RD_n           => sMFTA_RD_n,
      WR_n           => sMFTA_WR_n,
      SIWU           => sMFTA_SIWU,
      CLKOUT         => MFTA_CLKOUT,
      OE_n           => MFTA_OE_n,
      FIFO_Q         => sMFTA_FifoQ,
      FIFO_EMPTY     => sMFTA_FifoEmpty,
      FIFO_RDREQ     => sMFTA_FifoRdreq,
      FIFO_RDCLK     => sMFTA_FifoRdClk,
      CMD_FIFO_Q     => sMFTA_CmdFifoQ,
      CMD_FIFO_EMPTY => sMFTA_CmdFifoEmpty,
      CMD_FIFO_RDREQ => sMFTA_CmdFifoRdreq
      );

  -- B Channel defaults for now:
  sMFTB_D_out <= (OTHERS => '0');
  sMFTB_D_T   <= '1';                   -- inputs
  MFTB_WR_n   <= '1';
  MFTB_SIWU   <= '1';
  MFTB_OE_n   <= '1';

  -- use the "B" channel of the Motherboard FTDI module
  -- to generate a global reset by simply writing anything
  -- to it
  mftb_rst : PROCESS (sClk50, sClkLocked) IS
    VARIABLE timeout : integer RANGE 0 TO 7 := 0;
  BEGIN  -- PROCESS mftb_reset
    IF sClkLocked = '0' THEN
      -- WHEN DCM is not locked yet
      mftbState   <= MFTB0;
      timeout     := 0;
      sMftb_reset <= '0';
    ELSIF rising_edge(sClk50) THEN      -- rising clock edge
      sMftb_reset <= '0';
      CASE mftbState IS
        WHEN MFTB0 =>
          MFTB_RD_n <= '1';
          IF MFTB_RXF_n = '0' THEN
            MFTB_RD_n <= '0';
            mftbState <= MFTB1;
          END IF;
        WHEN MFTB1 =>
          MFTB_RD_n <= '0';
          mftbState <= MFTB2;
        WHEN MFTB2 =>
          MFTB_RD_n <= '1';
          IF MFTB_RXF_n = '1' THEN
            timeout   := 0;
            mftbState <= MFTB3;
          END IF;
        WHEN MFTB3 =>
          sMftb_reset <= '1';
          timeout     := timeout + 1;
          IF timeout = 7 THEN
            mftbState <= MFTB0;
          END IF;
        WHEN OTHERS =>
          mftbState <= MFTB0;
      END CASE;

    END IF;
  END PROCESS mftb_rst;

---------------------------------------------> MB FTDI

---------------------------------------------< Control Interface

  -- control interface module

  control_inst : control_interface
    PORT MAP (
      CLK             => sClk50,
      RESET           => sGlobalRST,
      FIFO_Q          => sMFTA_FifoQ,
      FIFO_EMPTY      => sMFTA_FifoEmpty,
      FIFO_RDREQ      => sMFTA_FifoRdreq,
      FIFO_RDCLK      => sMFTA_FifoRdClk,
      CMD_FIFO_Q      => sMFTA_CmdFifoQ,
      CMD_FIFO_EMPTY  => sMFTA_CmdFifoEmpty,
      CMD_FIFO_RDREQ  => sMFTA_CmdFifoRdreq,
      BUSY_RT         => sBusyRT, 
      FIFO_DATA       => sFIFODataRT,   
      FIFO_WREN       => sFIFOWrEnRT,    
      FIFO_FULL       => sFIFOFullRT,
      REG_DATA        => SRegDataRT, 
      REG_WREN        => sRegWrEnRT
      );   

  sStatusReg(255 DOWNTO 0) <= (OTHERS => '0');
---------------------------------------------> Control Interface

---------------------------------------------< Radiation tests Interface

  rad_test_interface : rad_test_int
    PORT MAP (
      CLK           => sClk50,
      FIFO_FULL     => sFIFOFullRT,
      REG_ADDR      => sMFTA_CmdFifoQ(25 DOWNTO 22),
      REG_IN        => sMFTA_CmdFifoQ(15 DOWNTO 0),
      REG_WREN      => sRegWrEnRT,                                            
      RST_N         => "NOT"(sGlobalRST),
		SPI_CLK       => sClk25,
      BUSY          => sBusyRT,
      FIFO_DATA_IN  => sFIFODataRT,   
      FIFO_WREN     => sFIFOWrEnRT,    
      REG_OUT       => SRegDataRT,    
      DC_A          => DC_A,
      DC_A_N        => DC_A_N,
      DC_A_P        => DC_A_P,
      DC_B          => DC_B
    );

---------------------------------------------> Radiation tests Interface

END behavior;