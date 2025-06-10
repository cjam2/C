<div style="max-width:900px; margin:40px auto; font-family:'Segoe UI', sans-serif; background:#f9f9f9; padding:40px; border-radius:8px; border:1px solid #ddd;">

  <!-- Page Title -->
  <h1 style="text-align:center; font-size:24px; margin-bottom:30px; color:#2e2e2e;">
    TD Internal Service Request Portal
  </h1>

  <!-- Form Box -->
  <div id="formWrapper" style="border:1px solid #d0d0d0; padding:30px; background:#fff; border-radius:8px;">
    <h2 style="margin-top:0; font-size:20px; color:#2e2e2e;">Service Request Form</h2>

    <div id="formFields">
      <div style="margin-bottom:15px; display:flex; align-items:center;">
        <label style="flex:0 0 220px; font-weight:600;">Request Classification<span style="color:red;"> *</span></label>
        <select class="required" style="flex:1; padding:8px; border:1px solid #ccc; border-radius:4px;">
          <option value="">Select classification</option>
          <option>Bug</option>
          <option>Enhancement</option>
          <option>New Feature</option>
        </select>
      </div>

      <div style="margin-bottom:15px; display:flex; align-items:center;">
        <label style="flex:0 0 220px; font-weight:600;">Request Type<span style="color:red;"> *</span></label>
        <select class="required" style="flex:1; padding:8px; border:1px solid #ccc; border-radius:4px;">
          <option value="">Select request type</option>
          <option>Standard</option>
          <option>Urgent</option>
        </select>
      </div>

      <h3 style="margin-top:30px; margin-bottom:10px; font-size:16px; color:#444;">Next Evolution of Work</h3>
      <hr style="border:none; border-top:1px solid #e2e2e2; margin-bottom:20px;">

      <div style="margin-bottom:15px; display:flex; align-items:center;">
        <label for="isNew" style="flex:0 0 220px; font-weight:600;">Is the project part of NEW<span style="color:red;"> *</span></label>
        <select id="isNew" class="required" style="flex:1; padding:8px; border:1px solid #ccc; border-radius:4px;">
          <option value="">Select one</option>
          <option value="Yes">Yes</option>
          <option value="No">No</option>
        </select>
      </div>

      <div id="platformContainer" style="margin-bottom:15px; display:none; display:flex; align-items:center;">
        <label style="flex:0 0 220px; font-weight:600;">Platform<span style="color:red;"> *</span></label>
        <select id="platform" class="required" style="flex:1; padding:8px; border:1px solid #ccc; border-radius:4px;">
          <option value="">Select platform</option>
          <option>Platform A</option>
          <option>Platform B</option>
          <option>Platform C</option>
        </select>
      </div>

      <div id="productGroupContainer" style="margin-bottom:15px; display:none; display:flex; align-items:center;">
        <label style="flex:0 0 220px; font-weight:600;">Product Group<span style="color:red;"> *</span></label>
        <select id="productGroup" class="required" style="flex:1; padding:8px; border:1px solid #ccc; border-radius:4px;">
          <option value="">Select product group</option>
        </select>
      </div>

      <div style="margin-bottom:15px; display:flex; align-items:center;">
        <label for="emailField" style="flex:0 0 220px; font-weight:600;">Email Address<span style="color:red;"> *</span></label>
        <input type="email" id="emailField" class="required" style="flex:1; padding:8px; border:1px solid #ccc; border-radius:4px;" readonly placeholder="Loading user email...">
      </div>
    </div>

    <div style="margin-top:30px; text-align:right;">
      <button onclick="submitForm()" style="padding:10px 20px; background:#0052cc; color:#fff; border:none; border-radius:4px; font-weight:600;">Submit</button>
    </div>
  </div>

  <!-- Footer / Thank you section -->
  <div style="text-align:center; margin-top:40px; font-size:13px; color:#777;">
    Powered by TD Internal Tools · Need Help? Contact <a href="mailto:support@td.com">support@td.com</a>
  </div>
</div>

<script>
  const isNew = document.getElementById('isNew');
  const platformContainer = document.getElementById('platformContainer');
  const productGroupContainer = document.getElementById('productGroupContainer');
  const platform = document.getElementById('platform');
  const productGroup = document.getElementById('productGroup');

  const groupMap = {
    'Platform A': ['Group A1', 'Group A2'],
    'Platform B': ['Group B1', 'Group B2'],
    'Platform C': ['Group C1', 'Group C2']
  };

  isNew.addEventListener('change', () => {
    if (isNew.value === 'Yes') {
      platformContainer.style.display = 'flex';
      platform.classList.add('required');
    } else {
      platformContainer.style.display = 'none';
      productGroupContainer.style.display = 'none';
      platform.classList.remove('required');
      productGroup.classList.remove('required');
    }
  });

  platform.addEventListener('change', () => {
    const selected = platform.value;
    const options = groupMap[selected] || [];
    productGroup.innerHTML = '<option value="">Select product group</option>';
    options.forEach(option => {
      const opt = document.createElement('option');
      opt.value = option;
      opt.text = option;
      productGroup.appendChild(opt);
    });
    if (options.length > 0) {
      productGroupContainer.style.display = 'flex';
      productGroup.classList.add('required');
    } else {
      productGroupContainer.style.display = 'none';
      productGroup.classList.remove('required');
    }
  });

  // Email auto-fill
  if (typeof AP !== 'undefined' && AP.user) {
    AP.user.getCurrentUser(function(user) {
      document.getElementById('emailField').value = user.email || 'Not available';
    });
  } else {
    document.getElementById('emailField').value = 'user@example.com';
  }

  // Validation logic
  function submitForm() {
    const required = document.querySelectorAll('.required');
    let valid = true;

    required.forEach(field => {
      if (!field.value) {
        field.style.border = '1px solid red';
        valid = false;
      } else {
        field.style.border = '1px solid #ccc';
      }
    });

    if (!valid) {
      alert('Please fill in all required fields.');
      return;
    }

    alert('Form submitted successfully!');
  }
</script>
