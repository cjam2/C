name: Say Hello

on:
  push:              # Runs on every push
  workflow_dispatch: # Allows manual trigger

jobs:
  say-hello:
    runs-on: ubuntu-latest

    steps:
    - name: Print greeting
      run: echo "Hello, world!"