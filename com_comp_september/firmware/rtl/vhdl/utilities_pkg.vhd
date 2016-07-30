-------------------------------------------------------------------------------
-- Title      : Utilities Package
-- Project    : HFT PXL
-------------------------------------------------------------------------------
-- File       : utilities_pkg.vhd
-- Author     : Joachim Schambach (jschamba@physics.utexas.edu)
-- Company    : University of Texas at Austin
-- Created    : 2012-07-24
-- Last update: 2014-02-12
-- Platform   : Windows, Xilinx ISE 13.4
-- Target     : Virtex-6 (XC6VLX240T-FF1759)
-- Standard   : VHDL'93/02
-------------------------------------------------------------------------------
-- Description: Several utility functions
-------------------------------------------------------------------------------
-- Copyright (c) 2012 
-------------------------------------------------------------------------------
-- Revisions  :
-- Date        Version  Author          Description
-- 2012-07-24  1.0      jschamba        Created
-------------------------------------------------------------------------------

LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE IEEE.numeric_std.ALL;
USE std.textio.ALL;

PACKAGE utilities IS
  TYPE ladderArray_type IS ARRAY (1 TO 4,                -- 4 ladders
                                  1 TO 10,               -- 10 sensors
                                  1 TO 2) OF std_logic;  -- 2 outputs

--  IMPURE FUNCTION svnversion_from_file(filename : IN string) RETURN std_logic_vector;

  FUNCTION crcUpdate(crc, data : std_logic_vector(31 DOWNTO 0)) RETURN std_logic_vector;
END PACKAGE utilities;

PACKAGE BODY utilities IS

  FUNCTION crcUpdate(crc, data : std_logic_vector(31 DOWNTO 0)) RETURN std_logic_vector IS
    VARIABLE x : std_logic_vector(31 DOWNTO 0);
    -- * Many web-refs on CRC: wikpedia, www.ciphersbyritter.com/ARTS/CRCMYST.HTM, Xilinx Appnotes
    -- * A reference on optimal CRC generators: Koopman,
    -- "32-Bit Cyclic Redundancy Codes for Internet Applications,"
    -- to appear in The International Conference on Dependable Systems and Networks (DSN) 2002.
    -- Seems (on my reading) to imply that the ethernet CRC generator is a good choice for our
    -- message length (~78kb)
    CONSTANT g : std_logic_vector(31 DOWNTO 0) := X"04c11db7";  -- ethernet CRC poly
  BEGIN
    x := crc;
    FOR i IN 31 DOWNTO 0 LOOP
      IF x(31) = data(i) THEN
        x := (x(30 DOWNTO 0)&'0') XOR g;
      ELSE
        x := (x(30 DOWNTO 0)&'0');
      END IF;
    END LOOP;
    RETURN x;
  END FUNCTION crcUpdate;
END PACKAGE BODY utilities;
