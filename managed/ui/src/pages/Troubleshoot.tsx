/*
 * Created on May 25 2023
 *
 * Copyright 2023 YugaByte, Inc. and Contributors
 */

import { lazy, Suspense } from 'react';
import { YBLoadingCircleIcon } from '../components/common/indicators';

const TroubleshootComponent = lazy(() =>
  import('../redesign/features/Troubleshoot/index').then(({ TroubleshootHeader }) => ({
    default: TroubleshootHeader
  }))
);

export const Troubleshoot = () => {
  return (
    <Suspense fallback={YBLoadingCircleIcon}>
      <TroubleshootComponent />
    </Suspense>
  );
};
