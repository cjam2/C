<div id="envExtraFields" style="display:none; margin-top:20px;">

  <div style="display:flex; margin-bottom:20px;">
    <label for="environmentRequired" style="flex:0 0 240px; font-weight:600;">
      Is Environment Needed?<span style="color:red;"> *</span>
    </label>
    <select id="environmentRequired" class="required-env" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;">
      <option value="">Select an option</option>
      <option value="Yes">Yes</option>
      <option value="No">No</option>
    </select>
  </div>

  <div style="display:flex; margin-bottom:20px;">
    <label for="environmentDescription" style="flex:0 0 240px; font-weight:600;">
      Environment Description<span style="color:red;"> *</span>
    </label>
    <input type="text" id="environmentDescription" class="required-env" 
           style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;" 
           placeholder="Describe the environment" />
  </div>

</div>
