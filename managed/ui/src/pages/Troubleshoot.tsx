/*
 * Created on May 25 2023
 *
 * Copyright 2023 YugaByte, Inc. and Contributors
 */

import { lazy, Suspense } from 'react';
import { YBLoadingCircleIcon } from '../components/common/indicators';

const TroubleshootComponent = lazy(() =>
  import('../redesign/features/Troubleshoot/index').then(({ TroubleshootTabs }) => ({
    default: TroubleshootTabs
  }))
);

export const Troubleshoot = () => {
  return (
    <Suspense fallback={YBLoadingCircleIcon}>
      <TroubleshootComponent />
    </Suspense>
  );
};
