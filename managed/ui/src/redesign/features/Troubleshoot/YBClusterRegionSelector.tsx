import { Box, MenuItem, makeStyles, Divider } from '@material-ui/core';
import { YBSelect } from '../../components';
import clsx from 'clsx';
import treeIcon from '../../../components/metrics/images/tree-icon.svg';

interface ClusterRegionSelectorProps {
  selectedItem: string;
  primaryClusterToRegionMap: any;
  asyncClusterToRegionMap: any;
  onClusterRegionSelected: (
    isCluster: boolean,
    isRegion: boolean,
    selectedOption: string,
    isPrimaryCluster: boolean
  ) => void;
}

const useStyles = makeStyles((theme) => ({
  selectBox: {
    minWidth: '250px'
  },
  ulItem: {
    paddingRight: '20px'
  },
  boldText: {
    fontWeight: 500
  },
  regularText: {
    fontWeight: 300
  },
  menuItem: {
    display: 'block',
    padding: '15px 20px',
    height: '52px',
    whiteSpace: 'nowrap',
    fontSize: '14px'
  },
  overrideMuiInput: {
    '& .MuiInput-input': {
      fontWeight: 300,
      fontSize: '14px'
    }
  },
  icon: {
    marginRight: theme.spacing(1)
  }
}));

const ALL_REGIONS = 'All Regions and Clusters';

export const ClusterRegionSelector = ({
  selectedItem,
  primaryClusterToRegionMap,
  asyncClusterToRegionMap,
  onClusterRegionSelected
}: ClusterRegionSelectorProps) => {
  const classes = useStyles();

  const renderClusterAndRegionItems = (
    primaryClusterToRegionMap: any,
    asyncClusterToRegionMap: any
  ) => {
    const renderedItems: any = [];

    renderedItems.push(
      <MenuItem
        key={ALL_REGIONS}
        value={ALL_REGIONS}
        onClick={(e: any) => {
          onClusterRegionSelected(false, false, ALL_REGIONS, false);
        }}
        className={clsx(classes.menuItem, classes.regularText)}
      >
        {ALL_REGIONS}
      </MenuItem>
    );

    renderedItems.push(<Divider />);

    // Add Primary Zones and Nodes
    for (const [cluster, clusterAttr] of primaryClusterToRegionMap.entries()) {
      renderedItems.push(
        <MenuItem
          key={clusterAttr.cluster}
          value={clusterAttr.cluster}
          onClick={(e: any) => {
            onClusterRegionSelected(true, false, clusterAttr.cluster, true);
          }}
          className={clsx(classes.menuItem, classes.boldText)}
        >
          {clusterAttr.cluster}
        </MenuItem>
      );
      clusterAttr.regions.forEach((region: string) => {
        renderedItems.push(
          <MenuItem
            key={region}
            value={region}
            onClick={(e: any) => {
              onClusterRegionSelected(false, true, region, true);
            }}
            className={clsx(classes.menuItem, classes.regularText)}
          >
            <img
              className={classes.icon}
              src={treeIcon}
              alt="Indicator towards metric measure to use"
            />
            {region}
          </MenuItem>
        );
      });
    }

    if (asyncClusterToRegionMap?.size > 0) {
      renderedItems.push(<Divider />);
    }
    // Add Read Replica Zones and Nodes
    // Add Primary Zones and Nodes
    for (const [cluster, clusterAttr] of asyncClusterToRegionMap.entries()) {
      renderedItems.push(
        <MenuItem
          key={clusterAttr.cluster}
          value={clusterAttr.cluster}
          onClick={(e: any) => {
            onClusterRegionSelected(true, false, clusterAttr.cluster, false);
          }}
          className={clsx(classes.menuItem, classes.boldText)}
        >
          {clusterAttr.cluster}
        </MenuItem>
      );
      clusterAttr.regions.forEach((region: string) => {
        renderedItems.push(
          <MenuItem
            key={region}
            value={region}
            onClick={(e: any) => {
              onClusterRegionSelected(false, true, region, false);
            }}
            className={clsx(classes.menuItem, classes.regularText)}
          >
            <img
              className={classes.icon}
              src={treeIcon}
              alt="Indicator towards metric measure to use"
            />
            {region}
          </MenuItem>
        );
      });
    }

    return renderedItems;
  };

  return (
    <YBSelect
      className={clsx(classes.selectBox, classes.ulItem, classes.overrideMuiInput)}
      data-testid="cluster-region-select"
      value={selectedItem}
    >
      {renderClusterAndRegionItems(primaryClusterToRegionMap, asyncClusterToRegionMap)}
    </YBSelect>
  );
};
