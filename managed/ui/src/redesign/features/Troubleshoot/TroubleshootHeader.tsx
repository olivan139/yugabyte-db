import React, { FC, useState } from 'react';
import { YBTab, YBTabs, YBTabPanel } from '../../components/YBTabs/YBTabs';
import { useTranslation } from 'react-i18next';
import { Box, Typography, makeStyles } from '@material-ui/core';
import { PGStatStatements } from './PGStatStatements';

const useStyles = makeStyles(() => ({
  tabLabel: {
    display: 'flex',
    flexDirection: 'row',
    alignItems: 'center'
  }
}));

export const NODE_AGENT_TABS = {
  AssignedNodes: 'assignedNodes',
  UnassignedNodes: 'unassignedNodes'
};

export const ServiceName = {
  METRICS: 'Metrics',
  PG_STAT_STATEMENTS: 'PGStatStatements',
  ACTIVE_SESSION_HISTORY: 'SessionHistory'
} as const;

export interface TSService {
  serviceName: typeof ServiceName[keyof typeof ServiceName];
}

export const TroubleshootHeader: FC<any> = () => {
  const { t } = useTranslation();
  const classes = useStyles();

  const [tab, setTabs] = useState<TSService['serviceName']>('Metrics');
  const handleChange = (_event: any, newValue: any) => {
    setTabs(newValue);
  };

  return (
    <Box sx={{ width: '100%' }}>
      <Box sx={{ borderBottom: 1, borderColor: 'divider' }}>
        <Typography variant="h2" className="content-title">
          {t('nodeAgent.title')}
        </Typography>
        <YBTabs value={tab} onChange={handleChange}>
          <YBTab
            label={<span className={classes.tabLabel}>{'Metrics'}</span>}
            value={'Metrics'}
            data-testid={`metrics-tab`}
          />
          <YBTab
            label={<span className={classes.tabLabel}>{'PG Stat Statements'}</span>}
            value={'PGStatStatements'}
            data-testid={`pg_stat_statements-tab`}
          />
          <YBTab
            label={<span className={classes.tabLabel}>{'Session History'}</span>}
            value={'SessionHistory'}
            data-testid={`session_history-tab`}
          />
        </YBTabs>
        <YBTabPanel value={(tab as unknown) as string} tabIndex={'Metrics' as any}>
          {'Hello, I am Metrics'}
        </YBTabPanel>
        <YBTabPanel value={(tab as unknown) as string} tabIndex={'PGStatStatements' as any}>
          <PGStatStatements />
        </YBTabPanel>
        <YBTabPanel value={(tab as unknown) as string} tabIndex={'SessionHistory' as any}>
          {'Hello, I am Active Session History'}
        </YBTabPanel>
      </Box>
    </Box>
  );
};
