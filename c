<div id="formWrapper" style="border:1px solid #ccc; padding:20px; max-width:700px; font-family:Arial, sans-serif;">
  <h2 style="margin-top:0;">Service Request Form</h2>

  <div style="margin-bottom:15px;">
    <label><strong>Request Classification<span style="color:red;"> *</span></strong></label><br>
    <select id="classification" style="width:100%; padding:8px;">
      <option value="">Select classification</option>
      <option>Bug</option>
      <option>Enhancement</option>
      <option>New Feature</option>
    </select>
  </div>

  <div style="margin-bottom:15px;">
    <label><strong>Request Type<span style="color:red;"> *</span></strong></label><br>
    <select id="requestType" style="width:100%; padding:8px;">
      <option value="">Select request type</option>
      <option>Standard</option>
      <option>Urgent</option>
    </select>
  </div>

  <h3 style="margin-bottom:5px;">Next Evolution of Work</h3>
  <hr style="border:none; border-top:1px solid #ddd;">

  <div style="margin-bottom:15px;">
    <label><strong>Is the project part of NEW<span style="color:red;"> *</span></strong></label><br>
    <select id="isNew" style="width:100%; padding:8px;">
      <option value="">Select one</option>
      <option value="Yes">Yes</option>
      <option value="No">No</option>
    </select>
  </div>

  <div id="platformContainer" style="margin-bottom:15px; display:none;">
    <label><strong>Platform<span style="color:red;"> *</span></strong></label><br>
    <select id="platform" style="width:100%; padding:8px;">
      <option value="">Select platform</option>
      <option>Platform A</option>
      <option>Platform B</option>
      <option>Platform C</option>
    </select>
  </div>

  <div id="productGroupContainer" style="margin-bottom:15px; display:none;">
    <label><strong>Product Group<span style="color:red;"> *</span></strong></label><br>
    <select id="productGroup" style="width:100%; padding:8px;">
      <option value="">Select product group</option>
    </select>
  </div>

  <div style="margin-bottom:15px;">
    <label><strong>Environment<span style="color:red;"> *</span></strong></label><br>
    <select style="width:100%; padding:8px;">
      <option>Select environment</option>
      <option>DEV</option>
      <option>QA</option>
      <option>UAT</option>
      <option>PROD</option>
    </select>
  </div>

  <div style="margin-bottom:15px;">
    <label><strong>Application Name<span style="color:red;"> *</span></strong></label><br>
    <input type="text" style="width:100%; padding:8px;" placeholder="Enter application name">
  </div>

  <div style="margin-bottom:5px;">
    <label><strong>MALCODE<span style="color:red;"> *</span></strong></label><br>
    <input type="text" style="width:100%; padding:8px;" placeholder="Enter MALCODE">
    <div style="font-size:12px; color:#666;">Please put 'NA' if malcode is unknown</div>
  </div>

  <div style="margin-bottom:5px;">
    <label><strong>Test Set<span style="color:red;"> *</span></strong></label><br>
    <input type="text" style="width:100%; padding:8px;" placeholder="Enter test set">
    <div style="font-size:12px; color:#666;">Please input 'NA' if test set not applicable</div>
  </div>

  <div style="margin-bottom:15px;">
    <label><strong>Version</strong></label><br>
    <input type="text" style="width:100%; padding:8px;" placeholder="Enter version">
  </div>

  <div style="margin-bottom:15px;">
    <label><strong>Email Address<span style="color:red;"> *</span></strong></label><br>
    <input type="email" id="emailField" style="width:100%; padding:8px;" readonly placeholder="Loading user email...">
  </div>

  <div style="margin-bottom:15px;">
    <label><strong>Target Start Date<span style="color:red;"> *</span></strong></label><br>
    <input type="date" style="width:100%; padding:8px;">
  </div>

  <div style="margin-bottom:15px;">
    <label><strong>Target End Date<span style="color:red;"> *</span></strong></label><br>
    <input type="date" style="width:100%; padding:8px;">
  </div>

  <div style="margin-bottom:15px;">
    <label><strong>Request Summary<span style="color:red;"> *</span></strong></label><br>
    <input type="text" style="width:100%; padding:8px;" placeholder="JIRA Notification Email subject">
  </div>

  <div style="margin-bottom:15px;">
    <label><strong>Request Details<span style="color:red;"> *</span></strong></label><br>
    <textarea style="width:100%; padding:8px;" rows="5" placeholder="Please enter issue details..."></textarea>
  </div>

  <button style="padding:10px 20px; background:#0052cc; color:#fff; border:none; border-radius:4px;">Submit</button>
</div>

<script>
  // Handle conditional dropdowns
  document.getElementById('isNew').addEventListener('change', function () {
    const show = this.value === 'Yes';
    document.getElementById('platformContainer').style.display = show ? 'block' : 'none';
    document.getElementById('productGroupContainer').style.display = 'none';
  });

  document.getElementById('platform').addEventListener('change', function () {
    const platform = this.value;
    const groups = {
      'Platform A': ['Group A1', 'Group A2'],
      'Platform B': ['Group B1', 'Group B2'],
      'Platform C': ['Group C1', 'Group C2']
    };
    const productSelect = document.getElementById('productGroup');
    const container = document.getElementById('productGroupContainer');

    productSelect.innerHTML = '<option value="">Select product group</option>';
    if (groups[platform]) {
      groups[platform].forEach(group => {
        const opt = document.createElement('option');
        opt.value = group;
        opt.text = group;
        productSelect.appendChild(opt);
      });
      container.style.display = 'block';
    } else {
      container.style.display = 'none';
    }
  });

  // Auto-fill current user email (Confluence macro-compatible)
  if (typeof AP !== 'undefined' && AP.user) {
    AP.user.getCurrentUser(function(user) {
      document.getElementById('emailField').value = user.email || 'Not available';
    });
  } else {
    document.getElementById('emailField').value = 'user@example.com'; // fallback
  }
</script>
