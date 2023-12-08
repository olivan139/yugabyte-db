import React, { useState } from 'react';
import { DiagnosticBox } from './DiagnosticBox';
import { formatData } from './TroubleshootUtils';
import { Box, makeStyles } from '@material-ui/core';

const MOCK_DATA = [
  {
    data: {
      entityType: 'NODE',
      isStale: false,
      new: false,
      observation:
        'Node yb-admin-test-master-placement-n2 processed 39410.69% more queries than average of other 2 nodes',
      recommendationPriority: 'MEDIUM',
      recommendationState: 'OPEN',
      recommendationInfo: {
        node_with_highest_query_load_details: {
          DeleteStmt: 24,
          InsertStmt: 15685,
          SelectStmt: 15702,
          UpdateStmt: 0
        },
        other_nodes_average_query_load_details: {
          DeleteStmt: 24,
          InsertStmt: 24,
          SelectStmt: 31.5,
          UpdateStmt: 0
        }
      },
      recommendationTimestamp: 1702047631.751105,
      suggestion: 'Redistribute queries to other nodes in the cluster.',
      target: 'yb-admin-test-master-placement-n2',
      type: 'QUERY_LOAD_SKEW'
    },
    key: 'QUERY_LOAD_SKEW-yb-admin-test-master-placement-n2-08cbf046a0505'
  }
];
interface TroubleshootAdvisorProps {
  universeUUID?: string;
  clusterUUID?: string;
  data: any;
}

const useStyles = makeStyles((theme) => ({
  test: {
    marginRight: theme.spacing(1)
  }
}));

interface RecommendationDetailProps {
  data: any;
  key: string;
  isResolved?: boolean;
}

export const TroubleshootAdvisor = ({
  data,
  universeUUID,
  clusterUUID
}: TroubleshootAdvisorProps) => {
  const formattedData = formatData(data);
  const classes = useStyles();
  console.warn('formattedData', formattedData, universeUUID, clusterUUID);
  const [displayedRecomendations, setDisplayedRecommendations] = useState<
    RecommendationDetailProps[]
  >(MOCK_DATA);

  const handleResolve = (id: string, isResolved: boolean) => {
    const copyRecommendations: any = [...MOCK_DATA];
    const userSelectedRecommendation = copyRecommendations.find((rec: any) => rec.key === id);
    if (userSelectedRecommendation) {
      userSelectedRecommendation.isResolved = isResolved;
    }
    setDisplayedRecommendations(copyRecommendations);
  };

  return (
    // This dialog is shown is when the last run API fails with 404
    <Box className={classes.test}>
      {displayedRecomendations.map((rec: any) => (
        <>
          <DiagnosticBox
            key={rec.key}
            idKey={rec.key}
            type={rec.data.type}
            data={rec.data}
            resolved={!!rec.isResolved}
            onResolve={handleResolve}
          />
        </>
      ))}
    </Box>
  );
};
