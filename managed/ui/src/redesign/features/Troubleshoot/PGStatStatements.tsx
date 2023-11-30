import React, { FC, useState } from 'react';
import { useTranslation } from 'react-i18next';
import { Box, MenuItem, makeStyles } from '@material-ui/core';
import { YBButton, YBLabel, YBSelect } from '../../components';

const UNIVERSE_MAP = ['Universe 1', 'Universe 2'];

const CLUSTER_MAP = ['Primary', 'Read Replica'];

const REGION_MAP = ['us-west2'];

const NODE_MAP = ['yb-node-1', 'yb-node-2'];

const useStyles = makeStyles((theme) => ({
  selectBox: {
    minWidth: '200px'
  }
}));

export const PGStatStatements: FC<any> = () => {
  const { t } = useTranslation();
  const classes = useStyles();
  const [universe, setUniverse] = useState<string>(UNIVERSE_MAP[0]);
  const [cluster, setCluster] = useState<string>(CLUSTER_MAP[0]);
  const [region, setRegion] = useState<string>(REGION_MAP[0]);
  const [node, setNode] = useState<string>(NODE_MAP[0]);

  return (
    <Box display="flex" flexDirection="row" mt={2}>
      <Box ml={2} display="flex" flexDirection="row">
        {/* <YBLabel width="fit-content">{'Select Universe: '}</YBLabel> */}
        <YBSelect
          className={classes.selectBox}
          // className={classes.select}
          data-testid="universe-select"
          onChange={(e) => {
            setUniverse(e.target.value);
          }}
          value={universe}
        >
          {UNIVERSE_MAP.map((universeName) => {
            return (
              <MenuItem key={universeName} value={universeName}>
                {universeName}
              </MenuItem>
            );
          })}
        </YBSelect>
      </Box>

      <Box ml={2}>
        {/* <YBLabel width="fit-content">{'Select Cluster Type: '}</YBLabel> */}
        <YBSelect
          className={classes.selectBox}
          // className={classes.select}
          data-testid="cluster-select"
          onChange={(e) => {
            setCluster(e.target.value);
          }}
          value={cluster}
        >
          {CLUSTER_MAP.map((clusterType) => {
            return (
              <MenuItem key={clusterType} value={clusterType}>
                {clusterType}
              </MenuItem>
            );
          })}
        </YBSelect>
      </Box>

      <Box ml={2}>
        {/* <YBLabel width="fit-content">{'Select Region: '}</YBLabel> */}
        <YBSelect
          className={classes.selectBox}
          // className={classes.select}
          data-testid="region-select"
          onChange={(e) => {
            setRegion(e.target.value);
          }}
          value={region}
        >
          {REGION_MAP.map((regionName) => {
            return (
              <MenuItem key={regionName} value={regionName}>
                {regionName}
              </MenuItem>
            );
          })}
        </YBSelect>
      </Box>

      <Box ml={2}>
        {/* <YBLabel width="fit-content">{'Select Node: '}</YBLabel> */}
        <YBSelect
          className={classes.selectBox}
          // className={classes.select}
          data-testid="node-select"
          onChange={(e) => {
            setNode(e.target.value);
          }}
          value={node}
        >
          {NODE_MAP.map((nodeName) => {
            return (
              <MenuItem key={nodeName} value={nodeName}>
                {nodeName}
              </MenuItem>
            );
          })}
        </YBSelect>
      </Box>

      <Box ml={2} mt={0.6}>
        <YBButton
          // className={classes.infoBannerActionButton}
          variant="primary"
          onClick={() => {
            console.warn('Hello');
          }}
          // startIcon={<SyncProblem fontSize="large" />}
          type="button"
          data-testid={`ResetFormButton`}
        >
          Run Diagnostics
        </YBButton>
      </Box>
    </Box>
  );
};
