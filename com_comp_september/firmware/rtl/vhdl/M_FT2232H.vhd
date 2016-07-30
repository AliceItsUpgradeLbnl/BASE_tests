-------------------------------------------------------------------------------
-- Title      : FTDI FT2232H Mini Module Interface on Motherboard
-- Project    : HFT PXL
-------------------------------------------------------------------------------
-- File       : M_FT2232H.vhd
-- Author     : Joachim Schambach
-- Company    : University of Texas
-- Created    : 2012-12-29
-- Last update: 2014-03-04
-- Platform   : Windows, Xilinx ISE 13.4
-- Target     : Virtex-6 (XC6VLX240T-FF1759)
-- Standard   : VHDL'93/02
-------------------------------------------------------------------------------
-- Description: Interface to the FT2232HQ USB minimodule on motherboard.
--              Protocol uses first 32bit word as "header" to determine
--              if READ or WRITE (from electronics to PC) and how many words
-------------------------------------------------------------------------------
-- Copyright (c) 2012 
-------------------------------------------------------------------------------
-- Revisions  :
-- Date        Version  Author          Description
-- 2012-12-29  1.0      jschamba        Created
-------------------------------------------------------------------------------

LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY M_FT2232H IS
  PORT (
    CLK            : IN  std_logic;     -- 50MHz clock input
    RESET          : IN  std_logic;     -- Active high reset
    -- FT2232H signals
    D_in           : IN  std_logic_vector(7 DOWNTO 0);   -- FIFO Data bus in
    D_out          : OUT std_logic_vector(7 DOWNTO 0);   -- FIFO Data bus out
    D_T            : OUT std_logic;     -- FIFO Data bus enable
    -- "C" Port
    RXF_n          : IN  std_logic;     -- Read enable
    TXE_n          : IN  std_logic;     -- Write enable
    RD_n           : OUT std_logic;     -- Read from USB FIFO
    WR_n           : OUT std_logic;     -- Write to USB FIFO
    SIWU           : OUT std_logic;     -- Send Immediate/Wake Up
    CLKOUT         : IN  std_logic;     -- Sync USB FIFO clock
    OE_n           : OUT std_logic;     -- Output enable (for sync FIFO)
    -- From FPGA to PC
    FIFO_Q         : IN  std_logic_vector(35 DOWNTO 0);  -- interface fifo data output port
    FIFO_EMPTY     : IN  std_logic;     -- interface fifo "emtpy" signal
    FIFO_RDREQ     : OUT std_logic;     -- interface fifo read request
    FIFO_RDCLK     : OUT std_logic;     -- interface fifo read clock
    -- From PC to FPGA
    CMD_FIFO_Q     : OUT std_logic_vector(35 DOWNTO 0);  -- interface command fifo data out port
    CMD_FIFO_EMPTY : OUT std_logic;  -- interface command fifo "emtpy" signal
    CMD_FIFO_RDREQ : IN  std_logic      -- interface command fifo read request
    );
END M_FT2232H;

ARCHITECTURE rtl OF M_FT2232H IS
  CONSTANT ZERO_18 : unsigned(17 DOWNTO 0) := "00" & x"0000";

  COMPONENT fifo16384x36
    PORT (
      rst    : IN  std_logic;
      wr_clk : IN  std_logic;
      rd_clk : IN  std_logic;
      din    : IN  std_logic_vector(35 DOWNTO 0);
      wr_en  : IN  std_logic;
      rd_en  : IN  std_logic;
      dout   : OUT std_logic_vector(35 DOWNTO 0);
      full   : OUT std_logic;
      empty  : OUT std_logic
      );
  END COMPONENT;

  SIGNAL sSiwu         : std_logic;
  SIGNAL sRd_n         : std_logic;     -- Read from FIFO
  SIGNAL sWr_n         : std_logic;     -- Write to FIFO
  SIGNAL sRxf_n        : std_logic;     -- Read Enable
  SIGNAL sTxe_n        : std_logic;     -- Write Enable
  SIGNAL sOe_n         : std_logic;     -- Output Enable
  --
  SIGNAL sCmdFifoWrClk : std_logic;
  SIGNAL sCmdFifoD     : std_logic_vector(35 DOWNTO 0);
  SIGNAL sRcvdData     : std_logic_vector(31 DOWNTO 0);
  SIGNAL sCmdFifoWrreq : std_logic;
  SIGNAL sCmdFifoFull  : std_logic;
  SIGNAL sLatchData    : std_logic;
  SIGNAL sRdCtr        : unsigned(15 DOWNTO 0);
  SIGNAL sWrCtr        : unsigned(17 DOWNTO 0);
  SIGNAL byteCtr       : integer RANGE 0 TO 3;

  TYPE rdState_type IS (
    RdStateIni0,
    RdStateIni1,
    RdStateIni1a,
    RdStateIni2,
    RdStateIni3,
    RdStateIni4,
    RdStateIni5,
    RdStateIni5a,
    RdStateIni6,
    RdStateIni7a,
    RdStateIni7b,
    RdStateIni8,
    RdState0,
    RdState1,
    RdState1a,
    RdState2,
    RdState3
    );
  SIGNAL rdState : rdState_type;

  TYPE wrState_type IS (
    WrState0,
    WrState1,
    WrState2,
    WrState3,
    WrState4
    );
  SIGNAL wrState : wrState_type;

-------------------------------------------------------------------------------
BEGIN

--------------------------------------------------------------------------------
-- Bidir I/O control
--------------------------------------------------------------------------------

  -- Ctrl Bus (C) buffers:
  -- C(0) : RXFn (I)
  -- C(1) : TXEn (I)
  -- C(2) : RD_n (O)
  -- C(3) : WR_n (O)
  -- C(4) : SIWU (O)
  -- C(5) : CLKOUT (I), only in sync FIFO mode
  -- C(6) : OE_n (O)
  -- C(7) : not connected

  -- inputs
  sRxf_n  <= RXF_n;
  sTxe_n  <= TXE_n;

  -- outputs
  RD_n <= sRd_n;  -- Read from channel A FIFO when nRD is low
  WR_n <= sWr_n;  -- Write to channel A FIFO when nWR is low
  SIWU <= sSiwu;  -- Used as "Send Immediate"
  OE_n <= sOe_n;  -- only in "sync FIFO" config

  -- logic starts here

  -- AD Bus buffers: Bi-directional
  D_T <= sOE_n;

  -- output FIFO word one byte at a time
  WITH sWrCtr(1 DOWNTO 0) SELECT
    D_out <=
    FIFO_Q(7 DOWNTO 0)   WHEN "00",
    FIFO_Q(15 DOWNTO 8)  WHEN "11",
    FIFO_Q(23 DOWNTO 16) WHEN "10",
    FIFO_Q(31 DOWNTO 24) WHEN OTHERS;

  -- event FIFO clock
  FIFO_RDCLK <= CLK;

  -- cmd FIFO
  sCmdFifoWrClk <= CLK;
  cmd_fifo : fifo16384x36
    PORT MAP (
      rst    => RESET,
      wr_clk => sCmdFifoWrClk,
      rd_clk => CLK,
      din    => sCmdFifoD,
      wr_en  => sCmdFifoWrreq,
      rd_en  => CMD_FIFO_RDREQ,
      dout   => CMD_FIFO_Q,
      full   => sCmdFifoFull,
      empty  => CMD_FIFO_EMPTY
      );
  sCmdFifoD(35 DOWNTO 32) <= "0000";    -- not used

  -----------------------------------------------------------------------------
  -- Read process, driven by RXFn going low
  -----------------------------------------------------------------------------
  rd_process : PROCESS (CLK, RESET) IS
  BEGIN  -- PROCESS rd_process
    IF RESET = '1' THEN                 -- asynchronous reset (active hi)
      rdState                <= RdStateIni0;
      sRd_n                  <= '1';
      sCmdFifoD(31 DOWNTO 0) <= (OTHERS => '0');
      SRcvdData              <= (OTHERS => '0');
      sCmdFifoWrreq          <= '0';
      byteCtr                <= 0;
      sLatchData             <= '0';
      sRdCtr                 <= (OTHERS => '0');
      
    ELSIF rising_edge(CLK) THEN         -- rising clock edge
      sCmdFifoWrreq <= '0';             -- default
      sLatchData    <= '0';

      IF sLatchData = '1' THEN
        sCmdFifoD(31 DOWNTO 0) <= sRcvdData;
        IF (sRcvdData(31 DOWNTO 16) = x"AAAA") AND (sRdCtr = x"0000") THEN
          -- start new "Read" transaction
          sCmdFifoWrreq <= '0';
          sRdCtr        <= unsigned(sRcvdData(15 DOWNTO 0));
        ELSIF ((sRdCtr = x"0000") AND NOT (sRcvdData(31 DOWNTO 16) = x"AAAB")) THEN
          sRdCtr        <= sRdCtr;
          sCmdFifoWrreq <= '0';
          -- this should never happen try to find sync again
          rdState       <= RdStateIni0;
        ELSE
          IF sRdCtr = x"00" THEN
            -- "Read" transaction finished
            sRdCtr        <= sRdCtr;
            sCmdFifoWrreq <= '0';
          ELSE
            -- "Read" transaction
            sRdCtr        <= sRdCtr - 1;
            sCmdFifoWrreq <= '1';
          END IF;
        END IF;
      END IF;

      CASE rdState IS
        WHEN RdStateIni0 =>
          sRd_n <= '1';
          IF sRxf_n = '0' THEN
            -- wait for "Read Ready" low
            rdState <= RdStateIni1;
          END IF;

        WHEN RdStateIni1 =>
          sRd_n   <= '0';               -- read it
          rdState <= RdStateIni1a;
          
        WHEN RdStateIni1a =>            -- make sure RD_N > 30ns
          sRd_n   <= '0';               -- read it
          rdState <= RdStateIni2;
          
        WHEN RdStateIni2 =>
          sRcvdData(31 DOWNTO 24) <= D_IN;                    -- latch byte
          sRcvdData(23 DOWNTO 0)  <= sRcvdData(31 DOWNTO 8);  -- shift it
          sRd_n                   <= '1';

          IF (sRxf_n = '0') THEN
            rdState <= RdStateIni3;
          ELSIF D_IN(7 DOWNTO 1) = "1010101" THEN
            rdState <= RdStateIni4;
          ELSE
            rdState <= RdStateIni0;
          END IF;
          
        WHEN RdStateIni3 =>
          sRd_n <= '1';
          -- wait for "Read Ready" to go back high
          IF sRxf_n = '0' THEN
            rdState <= RdStateIni3;
          ELSIF sRcvdData(31 DOWNTO 25) = "1010101" THEN  -- AA or AB
            rdState <= RdStateIni4;
          ELSE
            rdState <= RdStateIni0;
          END IF;

        WHEN RdStateIni4 =>
          sRd_n <= '1';
          IF sRxf_n = '0' THEN
            -- wait for "Read Ready" low
            rdState <= RdStateIni5;
          END IF;

        WHEN RdStateIni5 =>
          sRd_n   <= '0';               -- read it
          rdState <= RdStateIni5a;
          
        WHEN RdStateIni5a =>            -- make sure RD_N > 30ns
          sRd_n   <= '0';               -- read it
          rdState <= RdStateIni6;
          
        WHEN RdStateIni6 =>
          sRcvdData(31 DOWNTO 24) <= D_IN;                    -- latch byte
          sRcvdData(23 DOWNTO 0)  <= sRcvdData(31 DOWNTO 8);  -- shift it
          sRd_n                   <= '1';

          IF (sRxf_n = '0') AND (D_IN = x"aa") THEN
            byteCtr <= 0;
            rdState <= RdStateIni7a;
          ELSIF sRxf_n = '0' THEN
            rdState <= RdStateIni7b;
          ELSIF D_IN = x"aa" THEN
            byteCtr <= 0;
            rdState <= RdStateIni8;
          ELSE
            rdState <= RdStateIni0;
          END IF;
          
        WHEN RdStateIni7a =>
          sRd_n <= '1';
          -- wait for "Read Ready" to go back high
          IF sRxf_n = '0' THEN
            rdState <= RdStateIni7a;
          ELSIF sRcvdData(31 DOWNTO 24) = x"aa" THEN
            -- latch 32bit word
            sLatchData <= '1';
            rdState    <= RdState0;
          ELSE
            -- latch 32bit word
            sLatchData <= '1';
            rdState    <= RdStateIni0;
          END IF;
          
        WHEN RdStateIni7b =>
          sRd_n <= '1';
          -- wait for "Read Ready" to go back high
          IF sRxf_n = '0' THEN
            rdState <= RdStateIni7b;
          ELSIF sRcvdData(31 DOWNTO 24) = x"aa" THEN
            rdState <= RdState0;
          ELSE
            rdState <= RdStateIni0;
          END IF;
          
        WHEN RdStateIni8 =>
          IF sRxf_n = '0' THEN
            rdState <= RdStateIni8;
          ELSE
            sLatchData <= '1';
            rdState    <= RdState0;
          END IF;

        WHEN RdState0 =>
          sRd_n <= '1';
          IF (sRxf_n = '0') AND (sCmdFifoFull = '0') THEN
            -- wait for "Read Ready" low
            rdState <= RdState1;
          END IF;

        WHEN RdState1 =>
          sRd_n   <= '0';               -- read it
          rdState <= RdState1a;
          
        WHEN RdState1a =>               -- make sure RD_N > 30ns
          sRd_n   <= '0';               -- read it
          rdState <= RdState2;
          
        WHEN RdState2 =>
          sRcvdData(31 DOWNTO 24) <= D_IN;                    -- latch byte
          sRcvdData(23 DOWNTO 0)  <= sRcvdData(31 DOWNTO 8);  -- shift it
          sRd_n                   <= '1';

          -- If "Read Ready" still low, wait in the next state,
          -- otherwise, continue to next byte
          IF sRxf_n = '1' THEN
            IF byteCtr = 3 THEN
              -- latch 32bit word
              sLatchData <= '1';
              byteCtr    <= 0;
            ELSE
              byteCtr <= byteCtr + 1;
            END IF;
            rdState <= RdState0;
          ELSE
            rdState <= RdState3;
          END IF;
          
        WHEN RdState3 =>
          sRd_n <= '1';
          -- wait for "Read Ready" to go back high
          IF sRxf_n = '1' THEN
            IF byteCtr = 3 THEN
              -- latch 32bit word
              sLatchData <= '1';
              byteCtr    <= 0;
            ELSE
              byteCtr <= byteCtr + 1;
            END IF;
            rdState <= RdState0;
          ELSE
            rdState <= RdState3;
          END IF;
          
        WHEN OTHERS =>
          -- shouldn't happen
          rdState <= RdStateIni0;
      END CASE;
    END IF;
  END PROCESS rd_process;

  -----------------------------------------------------------------------------
  -- Write process, driven by TXEn going low
  -----------------------------------------------------------------------------
  wr_process : PROCESS (CLK, RESET) IS
  BEGIN  -- PROCESS write_process
    IF RESET = '1' THEN                 -- asynchronous reset (active high)
      sOE_n      <= '1';                -- default to input
      sWr_n      <= '1';
      sSiwu      <= '1';
      sWrCtr     <= (OTHERS => '0');
      FIFO_RDREQ <= '0';
      wrState    <= WrState0;
      
    ELSIF rising_edge(CLK) THEN
      sOE_n      <= '0';                -- default to ouput
      sWr_n      <= '1';
      sSiwu      <= '1';
      FIFO_RDREQ <= '0';

      CASE wrState IS
        WHEN WrState0 =>
          sOE_n <= '1';                 -- input while waiting
          IF (sRcvdData(31 DOWNTO 16) = x"AAAB") AND (sRdCtr = x"0000") AND (sLatchData = '1') THEN
            -- write counter is 32bit words to write times 4
            sWrCtr  <= unsigned(sRcvdData(15 DOWNTO 0)) & "00";
            wrState <= WrState1;
          END IF;
          
        WHEN WrState1 =>
          IF sRxf_n = '0' THEN
            -- back to reading again
            wrState <= WrState0;
          ELSIF sWrCtr = ZERO_18 THEN
            -- if 0 words are requested, also back to reading again
            wrState <= WrState0;
          ELSIF FIFO_EMPTY = '0' THEN
            -- otherwise, wait for FIFO not empty
            FIFO_RDREQ <= '1';
            wrState    <= wrState2;
          END IF;
          
        WHEN WrState2 =>
          sOE_n <= '0';
          IF sRxf_n = '0' THEN
            -- back to reading again
            wrState <= WrState0;
          ELSIF sTxe_n = '0' THEN
            -- wait for TX Enable low
            wrState <= WrState3;
          END IF;

        WHEN WrState3 =>
          sOE_n <= '0';
          sWr_n <= '0';                 -- indicate next byte ready
          IF sTxe_n = '1' THEN
            -- wait for TX Enable to go high (write accepted)
            sWrCtr  <= sWrCtr - 1;
            wrState <= WrState4;
          END IF;
          
        WHEN WrState4 =>
          IF sWrCtr = ZERO_18 THEN
            -- Finished with all requested WRITES
            sSiwu   <= '0';             -- "Send Immediate"
            wrState <= WrState0;
          ELSIF sWrCtr(1 DOWNTO 0) = "00" THEN
            -- get next FIFO word
            wrState <= wrState1;
          ELSE
            -- next byte write
            wrState <= WrState2;
          END IF;

        WHEN OTHERS =>
          -- should never happen
          wrState <= WrState0;
      END CASE;
      
    END IF;
  END PROCESS wr_process;
  
END rtl;
