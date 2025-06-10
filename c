<div id="formWrapper" style="border:1px solid #d0d0d0; padding:30px; max-width:700px; font-family:'Segoe UI', sans-serif; background:#fff; border-radius:8px;">
  <h2 style="margin-top:0; font-size:20px; color:#2e2e2e;">Service Request Form</h2>

  <div style="margin-bottom:20px;">
    <label style="font-weight:600;">Request Classification<span style="color:red;"> *</span></label><br>
    <select style="width:100%; padding:10px; border:1px solid #ccc; border-radius:4px;">
      <option value="">Select classification</option>
      <option>Bug</option>
      <option>Enhancement</option>
      <option>New Feature</option>
    </select>
  </div>

  <div style="margin-bottom:20px;">
    <label style="font-weight:600;">Request Type<span style="color:red;"> *</span></label><br>
    <select style="width:100%; padding:10px; border:1px solid #ccc; border-radius:4px;">
      <option value="">Select request type</option>
      <option>Standard</option>
      <option>Urgent</option>
    </select>
  </div>

  <h3 style="margin-top:30px; margin-bottom:10px; font-size:16px; color:#444;">Next Evolution of Work</h3>
  <hr style="border:none; border-top:1px solid #e2e2e2; margin-bottom:20px;">

  <div style="margin-bottom:20px;">
    <label style="font-weight:600;">Is the project part of NEW<span style="color:red;"> *</span></label><br>
    <select id="isNew" style="width:100%; padding:10px; border:1px solid #ccc; border-radius:4px;">
      <option value="">Select one</option>
      <option value="Yes">Yes</option>
      <option value="No">No</option>
    </select>
  </div>

  <div id="platformContainer" style="margin-bottom:20px; display:none;">
    <label style="font-weight:600;">Platform<span style="color:red;"> *</span></label><br>
    <select id="platform" style="width:100%; padding:10px; border:1px solid #ccc; border-radius:4px;">
      <option value="">Select platform</option>
      <option>Platform A</option>
      <option>Platform B</option>
      <option>Platform C</option>
    </select>
  </div>

  <div id="productGroupContainer" style="margin-bottom:20px; display:none;">
    <label style="font-weight:600;">Product Group<span style="color:red;"> *</span></label><br>
    <select id="productGroup" style="width:100%; padding:10px; border:1px solid #ccc; border-radius:4px;">
      <option value="">Select product group</option>
    </select>
  </div>

  <div style="margin-bottom:20px;">
    <label style="font-weight:600;">Environment<span style="color:red;"> *</span></label><br>
    <select style="width:100%; padding:10px; border:1px solid #ccc; border-radius:4px;">
      <option>Select environment</option>
      <option>DEV</option>
      <option>QA</option>
      <option>UAT</option>
      <option>PROD</option>
    </select>
  </div>

  <div style="margin-bottom:20px;">
    <label style="font-weight:600;">Application Name<span style="color:red;"> *</span></label><br>
    <input type="text" style="width:100%; padding:10px; border:1px solid #ccc; border-radius:4px;" placeholder="Enter application name">
  </div>

  <div style="margin-bottom:5px;">
    <label style="font-weight:600;">MALCODE<span style="color:red;"> *</span></label><br>
    <input type="text" style="width:100%; padding:10px; border:1px solid #ccc; border-radius:4px;" placeholder="Enter MALCODE">
    <div style="font-size:12px; color:#777;">Please put 'NA' if malcode is unknown</div>
  </div>

  <div style="margin-bottom:5px;">
    <label style="font-weight:600;">Test Set<span style="color:red;"> *</span></label><br>
    <input type="text" style="width:100%; padding:10px; border:1px solid #ccc; border-radius:4px;" placeholder="Enter test set">
    <div style="font-size:12px; color:#777;">Please input 'NA' if test set not applicable</div>
  </div>

  <div style="margin-bottom:20px;">
    <label style="font-weight:600;">Version</label><br>
    <input type="text" style="width:100%; padding:10px; border:1px solid #ccc; border-radius:4px;" placeholder="Enter version">
  </div>

  <div style="margin-bottom:20px;">
    <label style="font-weight:600;">Email Address<span style="color:red;"> *</span></label><br>
    <input type="email" id="emailField" style="width:100%; padding:10px; border:1px solid #ccc; border-radius:4px;" readonly placeholder="Loading user email...">
  </div>

  <div style="margin-bottom:20px;">
    <label style="font-weight:600;">Target Start Date<span style="color:red;"> *</span></label><br>
    <input type="date" style="width:100%; padding:10px; border:1px solid #ccc; border-radius:4px;">
  </div>

  <div style="margin-bottom:20px;">
    <label style="font-weight:600;">Target End Date<span style="color:red;"> *</span></label><br>
    <input type="date" style="width:100%; padding:10px; border:1px solid #ccc; border-radius:4px;">
  </div>

  <div style="margin-bottom:20px;">
    <label style="font-weight:600;">Request Summary<span style="color:red;"> *</span></label><br>
    <input type="text" style="width:100%; padding:10px; border:1px solid #ccc; border-radius:4px;" placeholder="JIRA Notification Email subject">
  </div>

  <div style="margin-bottom:25px;">
    <label style="font-weight:600;">Request Details<span style="color:red;"> *</span></label><br>
    <textarea style="width:100%; padding:10px; border:1px solid #ccc; border-radius:4px;" rows="5" placeholder="Please enter issue details..."></textarea>
  </div>

  <button style="padding:10px 22px; background:#0052cc; color:#fff; border:none; border-radius:4px; font-weight:600; font-size:14px;">Submit</button>
</div>

<script>
  // Dynamic dropdowns
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

  // Auto-fill current user email from Confluence
  if (typeof AP !== 'undefined' && AP.user) {
    AP.user.getCurrentUser(function(user) {
      document.getElementById('emailField').value = user.email || 'Not available';
    });
  } else {
    document.getElementById('emailField').value = 'user@example.com';
  }
</script>
