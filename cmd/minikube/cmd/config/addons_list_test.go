/*
Copyright 2021 The Kubernetes Authors All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

package config

import (
	"encoding/json"
	"os"
	"testing"

	"k8s.io/minikube/pkg/minikube/out"
)

func TestAddonsList(t *testing.T) {
	t.Run("NonExistingClusterTable", func(t *testing.T) {
		b := make([]byte, 167)
		r, w, err := os.Pipe()
		if err != nil {
			t.Fatalf("failed to create pipe: %v", err)
		}
		old := os.Stdout
		defer func() { os.Stdout = old }()
		os.Stdout = w
		printAddonsList(nil)
		if err := w.Close(); err != nil {
			t.Fatalf("failed to close pipe: %v", err)
		}
		if _, err := r.Read(b); err != nil {
			t.Fatalf("failed to read bytes: %v", err)
		}
		got := string(b)
		expected := `|-----------------------------|-----------------------|
|         ADDON NAME          |      MAINTAINER       |
|-----------------------------|-----------------------|`
		if got != expected {
			t.Errorf("Expected header to be: %q; got = %q", expected, got)
		}
	})

	t.Run("NonExistingClusterJSON", func(t *testing.T) {
		type addons struct {
			Ambassador *interface{} `json:"ambassador"`
		}

		b := make([]byte, 534)
		r, w, err := os.Pipe()
		if err != nil {
			t.Fatalf("failed to create pipe: %v", err)
		}
		old := os.Stdout
		defer func() {
			os.Stdout = old
			out.SetOutFile(os.Stdout)
		}()
		os.Stdout = w
		out.SetOutFile(os.Stdout)
		printAddonsJSON(nil)
		if err := w.Close(); err != nil {
			t.Fatalf("failed to close pipe: %v", err)
		}
		if _, err := r.Read(b); err != nil {
			t.Fatalf("failed to read bytes: %v", err)
		}
		got := addons{}
		if err := json.Unmarshal(b, &got); err != nil {
			t.Fatalf("failed to unmarshal output; output: %q; err: %v", string(b), err)
		}
		if got.Ambassador == nil {
			t.Errorf("expected `ambassador` field to not be nil, but was")
		}
	})
}
