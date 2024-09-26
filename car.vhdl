--Dichiarazione librerie
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;



--Dichiarazione entrate e uscite
entity Lattice is
	port(
		MPulse : in std_logic_vector (3 downto 0);
		SPulse : in std_logic_vector (4 downto 0);
		PWMMOT : out std_logic;
		PWMSER : out std_logic
		--RetroSwitch : out std_logic_vector(1 downto 0);
		);
end;


--Architettura
architecture comportamentale of Lattice is



--Dichiarazione componente
COMPONENT OSCH
	GENERIC ( NOM_FREQ: string := "10.23" ); --10.23MHz
	PORT ( 
		STDBY 	: in std_logic;
		SEDSTDBY	: OUT std_logic ;	
        	OSC 		: OUT std_logic );
		
END COMPONENT;

--Dichiarazione segnali
signal Clock: std_logic;
signal ContaM: integer range 0 to 1200;
signal ContaS: integer range 0 to 200000;
signal MPulse2: std_logic_vector (3 downto 0);
signal MPulse3: integer;
signal MPulse4: integer;
signal SPulse2: std_logic_vector (4 downto 0);
signal SPulse3: integer;
signal SPulse4: integer;



begin

OSCInst0 : OSCH
	GENERIC MAP( NOM_FREQ => "10.23" ) --10.23MHz
	PORT MAP ( STDBY => '0', OSC => Clock );

--processo
process(Clock)
begin
	
	if RISING_EDGE(Clock) then 
		--Contatori--
		if ContaM = 1000 then
			ContaM <= 0;
		else
			ContaM <= ContaM + 1;
		end if;
		
		if ContaS = 200000 then
			ContaS <= 0;
		else
			ContaS <= ContaS + 1;
		end if;
	
		--Controllo motori BLDC--
		if ContaM = 0 then
			PWMMOT <= '0';
		elsif ContaM = MPulse3 then
			PWMMOT <= '1';
		elsif ContaM = MPulse4 then
			PWMMOT <= '0';
		end if;
		
		MPulse2 <= not MPulse;
		MPulse3 <= conv_integer(MPulse2)* 10;
		MPulse4 <= MPulse3 * 2;
		
		--Controllo servo motore--
		if ContaS <= 0 then
			PWMSER <= '0';
		elsif ContaS = SPulse3 then
			PWMSER <= '1';
		elsif ContaS = SPulse4 then
			PWMSER <= '0';
		end if;
		
		SPulse2 <= not SPulse;
		SPulse3 <= conv_integer(SPulse2) * 1000;
		SPulse4 <= SPulse3 * 2;
		
		
	end if;	

end process;



end comportamentale;
