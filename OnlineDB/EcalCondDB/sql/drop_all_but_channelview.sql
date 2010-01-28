SET NEWPAGE 0
SET SPACE 0
SET LINESIZE 80
SET PAGESIZE 0
SET ECHO OFF
SET FEEDBACK OFF
SET HEADING OFF
SET MARKUP HTML OFF
SPOOL DELETEME.sql
select 'DROP TABLE ', TABLE_NAME, ' CASCADE CONSTRAINTS PURGE;' FROM USER_TABLES WHERE TABLE_NAME != 'CHANNELVIEW' AND TABLE_NAME NOT LIKE 'BIN%';
select 'DROP SEQUENCE ', SEQUENCE_NAME, ';'  FROM ALL_SEQUENCES WHERE SEQUENCE_OWNER LIKE 'CMS_ECAL_COND';
COMMIT;
SELECT 'SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME NOT LIKE ''BIN%'';' FROM DUAL;
SELECT 'SELECT SEQUENCE_NAME FROM ALL_SEQUENCES WHERE SEQUENCE_NAME NOT LIKE ''%$%'';' FROM DUAL;
SPOOL OFF
@DELETEME

