/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef DOM_LOCKS_LOCKREQUESTCHILD_H_
#define DOM_LOCKS_LOCKREQUESTCHILD_H_

#include "mozilla/dom/locks/PLockRequestChild.h"
#include "mozilla/dom/Lock.h"
#include "mozilla/dom/WorkerRef.h"
#include "nsISupportsImpl.h"

namespace mozilla::dom::locks {

struct LockRequest {
  nsString mName;
  RefPtr<Promise> mPromise;
  RefPtr<LockGrantedCallback> mCallback;
};

class LockRequestChild final : public PLockRequestChild,
                               public AbortFollower,
                               public SupportsWeakPtr {
  using IPCResult = mozilla::ipc::IPCResult;

  NS_DECL_ISUPPORTS

 public:
  explicit LockRequestChild(
      const LockRequest& aRequest,
      const Optional<OwningNonNull<AbortSignal>>& aSignal);

  IPCResult RecvResolve(const LockMode& aLockMode, bool aIsAvailable);
  IPCResult RecvAbort();

  void RunAbortAlgorithm() final;

 private:
  ~LockRequestChild() = default;

  LockRequest mRequest;
  RefPtr<StrongWorkerRef> mWorkerRef;
};

}  // namespace mozilla::dom::locks

#endif  // DOM_LOCKS_LOCKREQUESTCHILD_H_
