/*
	create_led_system
	created by Giovanni.Organtini@roma1.infn.it 2010

	stores data about LED system power and control
*/

/* channels */
DECLARE
	LOGIC_ID INTEGER := 1050000000;
	Y NUMBER := 1;
	X NUMBER := 1;
BEGIN
	INSERT INTO VIEWDESCRIPTION VALUES ('LED_PS_CHANNEL', 
				     'LED PS CHANNEL NUMBER',
				     'ENDCAP', 'DEE', 'CHANNEL');
	INSERT INTO VIEWDESCRIPTION VALUES ('LED_PM_CHANNEL', 
				     'LED PM CHANNEL NUMBER',
				     'ENDCAP', 'DEE', 'CHANNEL');
	INSERT INTO VIEWDESCRIPTION VALUES ('LED_CB_CHANNEL', 
				     'LED CB CHANNEL NUMBER',
				     'ENDCAP', 'DEE', 'CHANNEL');
	INSERT INTO VIEWDESCRIPTION VALUES ('LED_LED_CHANNEL', 
				     'LED LED CHANNEL NUMBER',
				     'ENDCAP', 'DEE', 'CHANNEL');

	FOR X IN 1..2 LOOP
	  FOR Y IN 1..10 LOOP
	    INSERT INTO CHANNELVIEW VALUES('LED_PS_CHANNEL', +1, X, Y,
		    		           'LED_PS_CHANNEL', LOGIC_ID);
	    LOGIC_ID := LOGIC_ID + 1;
	    INSERT INTO CHANNELVIEW VALUES('LED_PS_CHANNEL', -1, X, Y,
		  		           'LED_PS_CHANNEL', LOGIC_ID);
	    LOGIC_ID := LOGIC_ID + 1;
	  END LOOP;
	END LOOP;

	FOR X IN 1..2 LOOP
	  FOR Y IN 1..20 LOOP
	    INSERT INTO CHANNELVIEW VALUES('LED_PM_CHANNEL', +1, X, Y,
		    		           'LED_PM_CHANNEL', LOGIC_ID);
	    LOGIC_ID := LOGIC_ID + 1;
  	    INSERT INTO CHANNELVIEW VALUES('LED_PM_CHANNEL', -1, X, Y,
		  		           'LED_PM_CHANNEL', LOGIC_ID);
	    LOGIC_ID := LOGIC_ID + 1;
	  END LOOP;
        END LOOP;

	FOR X IN 1..2 LOOP
	  FOR Y IN 1..9 LOOP
	    INSERT INTO CHANNELVIEW VALUES('LED_CB_CHANNEL', +1, Y, NULL,
		  		           'LED_CB_CHANNEL', LOGIC_ID);
	    LOGIC_ID := LOGIC_ID + 1;
	    INSERT INTO CHANNELVIEW VALUES('LED_CB_CHANNEL', -1, Y, NULL,
		  		           'LED_CB_CHANNEL', LOGIC_ID);
	    LOGIC_ID := LOGIC_ID + 1;
	  END LOOP;
	END LOOP;

	FOR X IN 1..2 LOOP
	  FOR Y IN 1..4 LOOP
	    INSERT INTO CHANNELVIEW VALUES('LED_LED_CHANNEL', +1, X, Y,
		    		           'LED_LED_CHANNEL', LOGIC_ID);
	    LOGIC_ID := LOGIC_ID + 1;
  	    INSERT INTO CHANNELVIEW VALUES('LED_LED_CHANNEL', -1, X, Y,
		  		           'LED_LED_CHANNEL', LOGIC_ID);
	    LOGIC_ID := LOGIC_ID + 1;
	  END LOOP;
        END LOOP;
END;
/

/* Tags */
CREATE TABLE LED_SYSTEM_TAG (
	TAG_ID		NUMBER(10) NOT NULL,
	GEN_TAG		VARCHAR(100) NOT NULL,
	VERSION		INTEGER NOT NULL
);

CREATE SEQUENCE LED_SYSTEM_TAG_SQ INCREMENT BY 1 START WITH 1 NOCACHE;

ALTER TABLE LED_SYSTEM_TAG ADD CONSTRAINT LED_SYSTEM_TAG_PK PRIMARY 
	KEY (TAG_ID);
ALTER TABLE LED_SYSTEM_TAG ADD CONSTRAINT LED_SYSTEM_TAG_UK UNIQUE (GEN_TAG,
	VERSION);
/

/* The following table associates a unique id to each table or group of
   tables used in IOV table to define IOV per group of tables */
CREATE TABLE LED_DATTABLE_INDEX (
	ID		NUMBER(3) NOT NULL,
	TABLE_NAME	VARCHAR(25) NOT NULL,
	DESCRIPTION     VARCHAR(255)
);

ALTER TABLE LED_DATTABLE_INDEX ADD CONTRAINT LED_DATTABLE_INDEX_PK
        PRIMARY KEY (TABLE_NAME);
alter table led_dattable_index add constraint led_dattable_index_uk unique (table_name);


INSERT INTO LED_DATTABLE_INDEX VALUES (1, 'LED_POWER_SUPPLY_MON', '');
INSERT INTO LED_DATTABLE_INDEX VALUES (1, 'LED_POWER_MONBOX_MON', '');
INSERT INTO LED_DATTABLE_INDEX VALUES (1, 'LED_POWER_CTRLBOX_MON', '');
INSERT INTO LED_DATTABLE_INDEX VALUES (1, 'LED_POWER_LED_MON', '');
INSERT INTO LED_DATTABLE_INDEX VALUES (2, 'LED_POWER_SUPPLY_STATUS', '');
INSERT INTO LED_DATTABLE_INDEX VALUES (2, 'LED_POWER_MONBOX_STATUS', '');
INSERT INTO LED_DATTABLE_INDEX VALUES (2, 'LED_POWER_CTRLBOX_STATUS', '');

/* Tables to store interval of validities */
CREATE TABLE LED_SYSTEM_IOV (
	IOV_ID		NUMBER(10),
	DATTABLE_ID     NUMBER(3) NOT NULL,
	TAG_ID		NUMBER(10) NOT NULL,
	SINCE		DATE NOT NULL,
	TILL		DATE NOT NULL,
	DB_TIMESTAMP	TIMESTAMP DEFAULT SYSTIMESTAMP NOT NULL
);

CREATE SEQUENCE LED_SYSTEM_IOV_SQ INCREMENT BY 1 START WITH 1 NOCACHE;

ALTER TABLE LED_SYSTEM_IOV ADD CONSTRAINT LED_SYSTEM_IOV_PK 
	PRIMARY KEY (IOV_ID);

ALTER TABLE LED_SYSTEM_IOV ADD CONSTRAINT LED_SYSTEM_IOV_FK1 
	FOREIGN KEY (DATTABLE_ID) REFERENCES LED_DATTABLE_INDEX (ID);

CREATE INDEX LED_SYSTEM_IOV_IX ON LED_SYSTEM_IOV (SINCE, TILL);

CREATE OR REPLACE TRIGGER LED_SYSTEM_IOV_TG
	BEFORE INSERT ON LED_SYSTEM_IOV
	REFERENCING NEW AS newiov
	FOR EACH ROW
	CALL update_iov_dates_new('LED_SYSTEM_IOV', :newiov.dattable_id, 
        'SINCE', 'TILL', 
        :newiov.since,
	:newiov.till, :newiov.tag_id)
/

/* low frequency status tables */
CREATE TABLE LED_POWER_SUPPLY_MON (
	IOV_ID 			NUMBER(10) NOT NULL,
	LOGIC_ID		NUMBER(10),
	PS_VOLTAGE_SET_POINT	FLOAT,
	PS_STATUS		INTEGER,
	PS_STANDEVT_STATUS	INTEGER,
	PS_EXEC_ERR		INTEGER,
	PS_QUERY_ERR		INTEGER
);

ALTER TABLE LED_POWER_SUPPLY_MON ADD CONSTRAINT LED_POWER_SUPPLY_MON_PK
	PRIMARY KEY (IOV_ID, LOGIC_ID);
ALTER TABLE LED_POWER_SUPPLY_MON ADD CONSTRAINT LED_POWER_SUPPLY_MON_FK1 
	FOREIGN KEY (IOV_ID) REFERENCES LED_SYSTEM_IOV (IOV_ID);
 
CREATE TABLE LED_POWER_MONBOX_MON (
	IOV_ID 			NUMBER,
	LOGIC_ID		NUMBER(10),
	PM_VOLTAGE_LIM_1	FLOAT,
	PM_VOLTAGE_LIM_2	FLOAT,
	PM_VOLTAGE_LIM_3	FLOAT,
	PM_VOLTAGE_LIM_4	FLOAT,
	PM_CURRENT_LIMIT_1	FLOAT,
	PM_CURRENT_LIMIT_2	FLOAT,
	PM_CURRENT_LIMIT_3	FLOAT,
	PM_CURRENT_LIMIT_4	FLOAT
);

ALTER TABLE LED_POWER_MONBOX_MON ADD CONSTRAINT LED_POWER_MONBOX_MON_PK
	PRIMARY KEY (IOV_ID, LOGIC_ID);
ALTER TABLE LED_POWER_MONBOX_MON ADD CONSTRAINT LED_POWER_MONBOX_MON_FK1 
	FOREIGN KEY (IOV_ID) REFERENCES LED_SYSTEM_IOV (IOV_ID);

CREATE TABLE LED_POWER_CTRLBOX_MON (
	IOV_ID			NUMBER,
	LOGIC_ID		NUMBER(10),
	CB_OUT_DELAY_COARSE	INTEGER,
	CB_OUT_DELAY_FINE	FLOAT,
	CB_OUT_PULSE_WIDTH	INTEGER	
);

ALTER TABLE LED_POWER_CTRLBOX_MON ADD CONSTRAINT LED_POWER_CTRLBOX_MON_PK
	PRIMARY KEY (IOV_ID, LOGIC_ID);
ALTER TABLE LED_POWER_CTRLBOX_MON ADD CONSTRAINT LED_POWER_CTRLBOX_MON_FK1 
	FOREIGN KEY (IOV_ID) REFERENCES LED_SYSTEM_IOV (IOV_ID);

CREATE TABLE LED_POWER_LED_MON (
	IOV_ID		NUMBER,
	LOGIC_ID	NUMBER(10),
	STATUS		NUMBER(1)
);

ALTER TABLE LED_POWER_LED_MON ADD CONSTRAINT LED_POWER_LED_MON_PK
	PRIMARY KEY (IOV_ID, LOGIC_ID);
ALTER TABLE LED_POWER_LED_MON ADD CONSTRAINT LED_POWER_LED_MON_FK1 
	FOREIGN KEY (IOV_ID) REFERENCES LED_SYSTEM_IOV (IOV_ID);

/* status tables */
CREATE TABLE LED_POWER_SUPPLY_STATUS (
	IOV_ID           NUMBER(10) NOT NULL,
	LOGIC_ID         NUMBER(10),
	PS_CURRENT	 NUMBER,
	PS_VOLTAGE	 NUMBER,
	PS_DELTAV	 NUMBER,
	PS_CURRENT_LIMIT NUMBER
);

ALTER TABLE LED_POWER_SUPPLY_STATUS ADD CONSTRAINT LED_POWER_SUPPLY_STATUS_PK
	PRIMARY KEY (IOV_ID, LOGIC_ID);
ALTER TABLE LED_POWER_SUPPLY_STATUS ADD CONSTRAINT LED_POWER_SUPPLY_STATUS_FK1 
	FOREIGN KEY (IOV_ID) REFERENCES LED_SYSTEM_IOV (IOV_ID);

CREATE TABLE LED_POWER_MONBOX_STATUS (
	IOV_ID		NUMBER(10) NOT NULL,
	LOGIC_ID	INTEGER,
	PM_VOLTAGE_1	FLOAT,
	PM_VOLTAGE_2	FLOAT,
	PM_VOLTAGE_3	FLOAT,
	PM_VOLTAGE_4	FLOAT,
	PM_CURRENT_1 	FLOAT,
	PM_CURRENT_2 	FLOAT,
	PM_CURRENT_3 	FLOAT,
	PM_CURRENT_4 	FLOAT,
	PM_STATE	INTEGER /* This is an 8 bit integer encoding
				   4 2-bit status words */	
);

ALTER TABLE LED_POWER_MONBOX_STATUS ADD CONSTRAINT LED_POWER_MONBOX_STATUS_PK
	PRIMARY KEY (IOV_ID, LOGIC_ID);
ALTER TABLE LED_POWER_MONBOX_STATUS ADD CONSTRAINT LED_POWER_MONBOX_STATUS_FK1 
	FOREIGN KEY (IOV_ID) REFERENCES LED_SYSTEM_IOV (IOV_ID);
 
CREATE TABLE LED_POWER_CTRLBOX_STATUS (
	IOV_ID		NUMBER(10) NOT NULL,
	LOGIC_ID	NUMBER(10),
	CB_EXT_COUNT	INTEGER,
	CB_EXT_FREQ	FLOAT,
	CB_INT_COUNT	INTEGER,
	CB_INT_FREQ	FLOAT,
	CB_OUT_COUNT	INTEGER,
	CB_OUT_FREQ	FLOAT
);

ALTER TABLE LED_POWER_CTRLBOX_STATUS ADD CONSTRAINT LED_POWER_CTRLBOX_STATUS_PK
	PRIMARY KEY (IOV_ID, LOGIC_ID);
ALTER TABLE LED_POWER_CTRLBOX_STATUS ADD CONSTRAINT 
	LED_POWER_CTRLBOX_STATUS_FK1 
	FOREIGN KEY (IOV_ID) REFERENCES LED_SYSTEM_IOV (IOV_ID);
/


