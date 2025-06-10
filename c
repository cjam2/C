<div style="max-width:900px; margin:40px auto; font-family:'Segoe UI', sans-serif;">
  <div style="border:1px solid #ccc; border-radius:6px; background:#fff; padding:30px;">
    <h2 style="margin-top:0; margin-bottom:30px;">Service Request Form</h2>

    <!-- Field Row -->
    <div style="display:flex; margin-bottom:20px;">
      <label style="flex:0 0 240px; font-weight:600;">Request Classification<span style="color:red;"> *</span></label>
      <select class="required" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;">
        <option value="">Select classification</option>
        <option>Bug</option>
        <option>Enhancement</option>
        <option>New Feature</option>
      </select>
    </div>

    <div style="display:flex; margin-bottom:20px;">
      <label style="flex:0 0 240px; font-weight:600;">Request Type<span style="color:red;"> *</span></label>
      <select class="required" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;">
        <option value="">Select request type</option>
        <option>Standard</option>
        <option>Urgent</option>
      </select>
    </div>

    <h3 style="margin-top:30px; margin-bottom:5px;">Next Evolution of Work</h3>
    <hr style="border:none; border-top:1px solid #ddd; margin-bottom:20px;">

    <div style="display:flex; margin-bottom:20px;">
      <label style="flex:0 0 240px; font-weight:600;">Is the project part of NEW<span style="color:red;"> *</span></label>
      <select id="isNew" class="required" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;">
        <option value="">Select one</option>
        <option value="Yes">Yes</option>
        <option value="No">No</option>
      </select>
    </div>

    <div style="display:flex; margin-bottom:20px;">
      <label style="flex:0 0 240px; font-weight:600;">Environment<span style="color:red;"> *</span></label>
      <select class="required" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;">
        <option value="">Select environment</option>
        <option>DEV</option>
        <option>QA</option>
        <option>UAT</option>
        <option>PROD</option>
      </select>
    </div>

    <div style="display:flex; margin-bottom:20px;">
      <label style="flex:0 0 240px; font-weight:600;">Application Name<span style="color:red;"> *</span></label>
      <input type="text" class="required" placeholder="Enter application name" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;">
    </div>

    <div style="display:flex; margin-bottom:5px;">
      <label style="flex:0 0 240px; font-weight:600;">MALCODE<span style="color:red;"> *</span></label>
      <input type="text" class="required" placeholder="Enter MALCODE" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;">
    </div>
    <div style="margin-left:240px; font-size:12px; color:#666; margin-bottom:15px;">
      Please put 'NA' if malcode is unknown
    </div>

    <div style="display:flex; margin-bottom:5px;">
      <label style="flex:0 0 240px; font-weight:600;">Test Set<span style="color:red;"> *</span></label>
      <input type="text" class="required" placeholder="Enter test set" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;">
    </div>
    <div style="margin-left:240px; font-size:12px; color:#666; margin-bottom:15px;">
      Please input 'NA' if test set not applicable
    </div>

    <div style="display:flex; margin-bottom:20px;">
      <label style="flex:0 0 240px; font-weight:600;">Version</label>
      <input type="text" placeholder="Enter version" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;">
    </div>

    <div style="display:flex; margin-bottom:20px;">
      <label style="flex:0 0 240px; font-weight:600;">Email Address<span style="color:red;"> *</span></label>
      <input type="email" id="emailField" class="required" readonly placeholder="user@example.com" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;">
    </div>

    <div style="display:flex; margin-bottom:20px;">
      <label style="flex:0 0 240px; font-weight:600;">Target Start Date<span style="color:red;"> *</span></label>
      <input type="date" class="required" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;">
    </div>

    <div style="display:flex; margin-bottom:20px;">
      <label style="flex:0 0 240px; font-weight:600;">Target End Date<span style="color:red;"> *</span></label>
      <input type="date" class="required" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;">
    </div>

    <div style="display:flex; margin-bottom:20px;">
      <label style="flex:0 0 240px; font-weight:600;">Request Summary<span style="color:red;"> *</span></label>
      <input type="text" class="required" placeholder="JIRA Notification Email subject (include Test Set)" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;">
    </div>

    <div style="display:flex; margin-bottom:5px;">
      <label style="flex:0 0 240px; font-weight:600;">Request Details<span style="color:red;"> *</span></label>
      <textarea class="required" placeholder="Please enter issue details and Error info or Error Stack" style="flex:1; padding:8px; border-radius:4px; border:1px solid #ccc;" rows="4"></textarea>
    </div>

    <div style="margin-left:240px; font-size:12px; color:#666; margin-bottom:25px;">
      Please enter issue details and Error info or Error Stack
    </div>

    <!-- File Upload Note -->
    <div style="margin-left:0; font-size:13px; margin-bottom:25px;">
      <strong>Please upload any related documents into the Jira tickets directly after submitting this form</strong> (i.e. XLS, PID, ABP, WSP, MKS)
    </div>

    <div style="text-align:right;">
      <button onclick="submitForm()" style="padding:10px 20px; background:#0052cc; color:white; border:none; border-radius:4px;">Submit</button>
    </div>
  </div>
</div>

<script>
  // Auto-fill email
  if (typeof AP !== 'undefined' && AP.user) {
    AP.user.getCurrentUser(function(user) {
      document.getElementById('emailField').value = user.email || 'Not available';
    });
  }

  function submitForm() {
    const fields = document.querySelectorAll('.required');
    let valid = true;

    fields.forEach(f => {
      if (!f.value) {
        f.style.border = '1px solid red';
        valid = false;
      } else {
        f.style.border = '1px solid #ccc';
      }
    });

    if (!valid) {
      alert('Please fill in all required fields.');
      return;
    }

    alert('Form submitted successfully!');
  }
</script>
