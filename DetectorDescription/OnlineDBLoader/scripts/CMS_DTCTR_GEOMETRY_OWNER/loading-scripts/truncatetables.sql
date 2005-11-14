truncate table CMSINTEGRATION.BOOLEANSOLIDS;
truncate table CMSINTEGRATION.BOXES;
truncate table CMSINTEGRATION.CATEGORIES;
truncate table CMSINTEGRATION.COMPOSITEMATERIALS;
truncate table CMSINTEGRATION.CONES;
truncate table CMSINTEGRATION.ELEMENTARYMATERIALS;
-- due to use of this table by other database schemas,
-- this needs to be a special delete, not truncate table CMSINTEGRATION.LOGICALPARTTYPES;
truncate table CMSINTEGRATION.DETECTORPARTS;
truncate table CMSINTEGRATION.MATERIALFRACTIONS;
truncate table CMSINTEGRATION.NOMINALPLACEMENTS;
truncate table CMSINTEGRATION.PHYSICALPARTSTREE;
truncate table CMSINTEGRATION.POLYCONES;
truncate table CMSINTEGRATION.POLYHEDRA;
truncate table CMSINTEGRATION.POSPARTSGRAPH;
truncate table CMSINTEGRATION.PSEUDOTRAPEZOIDS;
truncate table CMSINTEGRATION.ROTATIONS;

alter table BOOLEANSOLIDS DISABLE CONSTRAINT  BOO_SOL_FK;
alter table BOOLEANSOLIDS DISABLE CONSTRAINT  BOO_SOL_FK_LEFTSOLIDNAME_FK;
alter table BOOLEANSOLIDS DISABLE CONSTRAINT  BOO_SOL_FK_RIGHTSOLIDNAME_FK;
alter table BOXES DISABLE CONSTRAINT  BOX_SOL_FK;
alter table CONES DISABLE CONSTRAINT  CON_SOL_FK;
alter table DETECTORPARTS DISABLE CONSTRAINT  DLP_SOL_FK;
alter table POLYCONES DISABLE CONSTRAINT  POL_SOL_FK;     
alter table POLYHEDRA DISABLE CONSTRAINT  POLH_SOL_FK;
alter table PSEUDOTRAPEZOIDS DISABLE CONSTRAINT  PSE_SOL_FK;
alter table REFLECTIONSOLIDS DISABLE CONSTRAINT  REF_SOL_FK;        
alter table REFLECTIONSOLIDS DISABLE CONSTRAINT  REFFK_REFLECTIONSOLIDSOLIDN_FK; 
alter table TRAPEZOIDS DISABLE CONSTRAINT  TRA_SOL_FK; 
alter table TRD1S DISABLE CONSTRAINT  TRD_SOL_FK; 
alter table TUBES DISABLE CONSTRAINT  TUB1_SOL_FK;    

truncate table CMSINTEGRATION.SOLIDS;

alter table BOOLEANSOLIDS ENABLE CONSTRAINT  BOO_SOL_FK;
alter table BOOLEANSOLIDS ENABLE CONSTRAINT  BOO_SOL_FK_LEFTSOLIDNAME_FK;
alter table BOOLEANSOLIDS ENABLE CONSTRAINT  BOO_SOL_FK_RIGHTSOLIDNAME_FK;
alter table BOXES ENABLE CONSTRAINT  BOX_SOL_FK;
alter table CONES ENABLE CONSTRAINT  CON_SOL_FK;
alter table DETECTORPARTS ENABLE CONSTRAINT  DLP_SOL_FK;
alter table POLYCONES ENABLE CONSTRAINT  POL_SOL_FK;     
alter table POLYHEDRA ENABLE CONSTRAINT  POLH_SOL_FK;
alter table PSEUDOTRAPEZOIDS ENABLE CONSTRAINT  PSE_SOL_FK;
alter table REFLECTIONSOLIDS ENABLE CONSTRAINT  REF_SOL_FK;        
alter table REFLECTIONSOLIDS ENABLE CONSTRAINT  REFFK_REFLECTIONSOLIDSOLIDN_FK; 
alter table TRAPEZOIDS ENABLE CONSTRAINT  TRA_SOL_FK; 
alter table TRD1S ENABLE CONSTRAINT  TRD_SOL_FK; 
alter table TUBES ENABLE CONSTRAINT  TUB1_SOL_FK;    


truncate table CMSINTEGRATION.TRAPEZOIDS;
truncate table CMSINTEGRATION.TRD1S;
truncate table CMSINTEGRATION.TUBES;
truncate table CMSINTEGRATION.ZSECTIONS;

--COMPOSITEMATERIALS             COM_MAT_FK
--DETECTORPARTS                  DLP_MAT_FK
--ELEMENTARYMATERIALS            ELE_MAT_FK
--MATERIALFRACTIONS              MATFRAC_MAT_FK
--SOLIDS                         SOL_MAT_FK
alter table CMSINTEGRATION.MATERIALFRACTIONS disable constraint MATFRAC_MAT_FK;
alter table CMSINTEGRATION.ELEMENTARYMATERIALS disable constraint ELE_MAT_FK;
alter table CMSINTEGRATION.SOLIDS disable constraint SOL_MAT_FK;
alter table CMSINTEGRATION.DETECTORPARTS disable constraint DLP_MAT_FK;
alter table CMSINTEGRATION.COMPOSITEMATERIALS disable constraint COM_MAT_FK;
truncate table CMSINTEGRATION.MATERIALS;
alter table CMSINTEGRATION.MATERIALFRACTIONS enable constraint MATFRAC_MAT_FK;
alter table CMSINTEGRATION.ELEMENTARYMATERIALS enable constraint ELE_MAT_FK;
alter table CMSINTEGRATION.SOLIDS enable constraint SOL_MAT_FK;
alter table CMSINTEGRATION.DETECTORPARTS enable constraint DLP_MAT_FK;
alter table CMSINTEGRATION.COMPOSITEMATERIALS enable constraint COM_MAT_FK;

-- WARNING:  This needs to be updated if namespaces from XML change.
delete from CMSINTEGRATION.LOGICALPARTTYPES where lpname like 'vcal%' or  lpname like 'hcal%' or  lpname like 'cms%' or  lpname like 'beampipe%' or  lpname like 'cavern%' or lpname like 'muon%' or lpname like 'mf%' or lpname like 'mb%' or lpname like 'tec%' or lpname like 'tob%' or lpname like 'tib%' or lpname like 'tid%' or lpname like 'ecal%' or lpname like 'pxlx%' or lpname like 'quadrupole%' or lpname like 'trak%' or lpname like 'malg%' or lpname like 'mgnt%' or lpname like 'MagneticFieldVolumes:%';
