name: Fruit Action
inputs:
  fruit:
    description: "Pick a fruit"
    required: true
    default: "apple"
    type: choice
    options:
      - apple
      - banana
      - orange
runs:
  using: "composite"
  steps:
    - shell: bash
      run: |
        if [ "${{ inputs.fruit }}" = "apple" ]; then
          echo "FRUIT=${{ secrets.FRUIT_APPLE }}" >> $GITHUB_ENV
        elif [ "${{ inputs.fruit }}" = "banana" ]; then
          echo "FRUIT=${{ secrets.FRUIT_BANANA }}" >> $GITHUB_ENV
        elif [ "${{ inputs.fruit }}" = "orange" ]; then
          echo "FRUIT=${{ secrets.FRUIT_ORANGE }}" >> $GITHUB_ENV
        fi
    - shell: bash
      run: ./test "$FRUIT"