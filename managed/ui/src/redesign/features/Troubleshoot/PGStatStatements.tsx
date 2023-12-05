import React, { useState } from 'react';
import { useUpdateEffect } from 'react-use';
import { Box } from '@material-ui/core';
import { YBButton } from '../../components';
import { formatHeaderData, getFilteredItems } from './TroubleshootUtils';

import { ClusterRegionSelector } from './YBClusterRegionSelector';
import { ZoneNodeSelector } from './YBZoneNodeSelector';

interface PGStatStatementsProps {
  data: any;
}

const INPUT_DATE_FORMAT = 'MM/dd/yyyy';
const ALL = 'All';
const ALL_REGIONS = 'All Regions and Clusters';
const ALL_ZONES = 'All Zones and Nodes';

export const PGStatStatements = ({ data }: PGStatStatementsProps) => {
  const {
    primaryZoneMapping,
    asyncZoneMapping,
    primaryClusterMapping,
    asyncClusterMapping
  } = formatHeaderData(data);

  const [clusterRegionItem, setClusterRegionItem] = useState(ALL_REGIONS);
  const [zoneNodeItem, setZoneNodeItem] = useState(ALL_ZONES);
  const [isPrimaryCluster, setIsPrimaryCluster] = useState(true);

  const [primaryZoneToNodesMap, setPrimaryZoneToNodesMap] = useState(primaryZoneMapping);
  const [asyncZoneToNodesMap, setAsyncZoneToNodesMap] = useState(asyncZoneMapping);

  const [cluster, setCluster] = useState(ALL);
  const [region, setRegion] = useState(ALL);
  const [zone, setZone] = useState(ALL);
  const [node, setNode] = useState(ALL);

  useUpdateEffect(() => {
    const emptyMap = new Map();
    if (region !== ALL || cluster !== ALL) {
      const filteredZoneToNodesMap = getFilteredItems(
        isPrimaryCluster ? primaryZoneMapping : asyncZoneMapping,
        region !== ALL,
        isPrimaryCluster,
        region !== ALL ? region : cluster
      );

      if (region !== null && isPrimaryCluster) {
        setPrimaryZoneToNodesMap(filteredZoneToNodesMap);
        setAsyncZoneToNodesMap(emptyMap);
      } else if (region !== null && !isPrimaryCluster) {
        setPrimaryZoneToNodesMap(emptyMap);
        setAsyncZoneToNodesMap(filteredZoneToNodesMap);
      } else if (cluster !== null && isPrimaryCluster) {
        setPrimaryZoneToNodesMap(filteredZoneToNodesMap);
        setAsyncZoneToNodesMap(emptyMap);
      } else if (cluster !== null && !isPrimaryCluster) {
        setPrimaryZoneToNodesMap(emptyMap);
        setAsyncZoneToNodesMap(filteredZoneToNodesMap);
      }
    } else {
      setPrimaryZoneToNodesMap(primaryZoneMapping);
      setAsyncZoneToNodesMap(asyncZoneMapping);
    }
  }, [region, cluster]);

  const onClusterRegionSelected = (
    isCluster: boolean,
    isRegion: boolean,
    selectedOption: string,
    isPrimaryCluster: boolean
  ) => {
    setIsPrimaryCluster(isPrimaryCluster);
    if (selectedOption === ALL_REGIONS) {
      setClusterRegionItem(ALL_REGIONS);
      setCluster(ALL);
      setRegion(ALL);
    }

    if (isCluster || isRegion) {
      setClusterRegionItem(selectedOption);
      console.warn('selectedOption', selectedOption);

      if (isCluster) {
        setCluster(selectedOption);
        setRegion(ALL);
      }

      if (isRegion) {
        setRegion(selectedOption);
        setCluster(ALL);
      }
    }
  };

  const onZoneNodeSelected = (isZone: boolean, isNode: boolean, selectedOption: string) => {
    if (selectedOption === ALL_ZONES) {
      setZoneNodeItem(ALL_ZONES);
      setZone(ALL);
      setNode(ALL);
    }

    if (isZone || isNode) {
      setZoneNodeItem(selectedOption);
      isZone ? setZone(selectedOption) : setNode(selectedOption);
    }
  };

  return (
    <Box display="flex" flexDirection="row" mt={2}>
      <Box ml={2}>
        <ClusterRegionSelector
          selectedItem={clusterRegionItem}
          onClusterRegionSelected={onClusterRegionSelected}
          primaryClusterToRegionMap={primaryClusterMapping}
          asyncClusterToRegionMap={asyncClusterMapping}
        />
      </Box>

      <Box ml={2}>
        <ZoneNodeSelector
          selectedItem={zoneNodeItem}
          onZoneNodeSelected={onZoneNodeSelected}
          primaryZoneToNodesMap={primaryZoneToNodesMap}
          asyncZoneToNodesMap={asyncZoneToNodesMap}
        />
      </Box>

      <Box ml={2} mt={0.6}>
        <YBButton variant="primary" type="button" data-testid={`DiagnosticButton`}>
          Run Diagnostics
        </YBButton>
      </Box>
    </Box>
  );
};
