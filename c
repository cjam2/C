name: Run Shell Script with Env Vars

on:
  schedule:
    - cron: '0 3 * * *'
  workflow_dispatch:

jobs:
  run-script:
    runs-on: ubuntu-latest
    env:
      PARAMS: "--env dev --ip 10.0.0.1 --user admin --enable-feature-x"
      # Or: PARAMS: ${{ secrets.MY_PARAMS }}

    steps:
    - name: Checkout repository
      uses: actions/checkout@v3

    - name: Make script executable
      run: chmod +x ./scripts/run_me.sh

    - name: Run script with env parameters
      run: ./scripts/run_me.sh $PARAMS