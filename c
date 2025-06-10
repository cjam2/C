<html>
<head>
  <style>
    #confluenceFormWrapper {
      max-width: 700px;
      margin: 20px auto;
      background: #fff;
      padding: 30px;
      border: 1px solid #ccc;
      border-radius: 6px;
      font-family: Arial, sans-serif;
    }
    .form-group {
      margin-bottom: 18px;
    }
    label {
      display: block;
      margin-bottom: 5px;
      font-weight: bold;
    }
    .required::after {
      content: " *";
      color: red;
    }
    input, select, textarea {
      width: 100%;
      padding: 10px;
      font-size: 14px;
      border: 1px solid #ccc;
      border-radius: 4px;
    }
    textarea {
      height: 100px;
      resize: vertical;
    }
    .hint {
      font-size: 12px;
      color: #666;
      margin-top: 4px;
    }
    h2 {
      margin-top: 0;
    }
    hr {
      border: none;
      border-top: 1px solid #ddd;
      margin: 5px 0 20px;
    }
    button {
      padding: 10px 18px;
      background-color: #0052cc;
      color: #fff;
      border: none;
      border-radius: 4px;
      font-size: 14px;
      cursor: pointer;
    }
    button:hover {
      background-color: #003d99;
    }
    .hidden {
      display: none;
    }
    .error {
      border-color: red;
    }
  </style>
</head>
<body>
  <div id="confluenceFormWrapper">
    <form id="serviceForm" novalidate>
      <h2>Service Request Form</h2>

      <div class="form-group">
        <label class="required">Request Classification</label>
        <select required>
          <option value="">Select classification</option>
          <option>Bug</option>
          <option>Enhancement</option>
          <option>New Feature</option>
        </select>
      </div>

      <div class="form-group">
        <label class="required">Request Type</label>
        <select required>
          <option value="">Select request type</option>
          <option>Standard</option>
          <option>Urgent</option>
        </select>
      </div>

      <h3 style="margin: 20px 0 5px;">Next Evolution of Work</h3>
      <hr />

      <div class="form-group">
        <label class="required">Is the project part of NEW</label>
        <select id="isNew" required>
          <option value="">Select one</option>
          <option value="Yes">Yes</option>
          <option value="No">No</option>
        </select>
      </div>

      <div class="form-group hidden" id="platformContainer">
        <label class="required">Platform</label>
        <select id="platform">
          <option value="">Select platform</option>
          <option>Platform A</option>
          <option>Platform B</option>
          <option>Platform C</option>
        </select>
      </div>

      <div class="form-group hidden" id="productGroupContainer">
        <label class="required">Product Group</label>
        <select id="productGroup">
          <option value="">Select product group</option>
        </select>
      </div>

      <div class="form-group">
        <label class="required">Environment</label>
        <select required>
          <option value="">Select environment</option>
          <option>DEV</option>
          <option>QA</option>
          <option>UAT</option>
          <option>PROD</option>
        </select>
      </div>

      <div class="form-group">
        <label class="required">Application Name</label>
        <input type="text" required placeholder="Enter application name">
      </div>

      <div class="form-group">
        <label class="required">MALCODE</label>
        <input type="text" required placeholder="Enter MALCODE">
        <div class="hint">Please put 'NA' if malcode is unknown</div>
      </div>

      <div class="form-group">
        <label class="required">Test Set</label>
        <input type="text" required placeholder="Enter test set">
        <div class="hint">Please input 'NA' if test set not applicable</div>
      </div>

      <div class="form-group">
        <label>Version</label>
        <input type="text" placeholder="Enter version">
      </div>

      <div class="form-group">
        <label class="required">Email Address</label>
        <input type="email" id="emailField" required readonly placeholder="Loading user email...">
      </div>

      <div class="form-group">
        <label class="required">Target Start Date</label>
        <input type="date" required>
      </div>

      <div class="form-group">
        <label class="required">Target End Date</label>
        <input type="date" required>
      </div>

      <div class="form-group">
        <label class="required">Request Summary</label>
        <input type="text" required placeholder="JIRA Notification Email subject (include Test Set)">
      </div>

      <div class="form-group">
        <label class="required">Request Details</label>
        <textarea required placeholder="Please enter issue details and Error info or Error Stack"></textarea>
      </div>

      <button type="submit">Submit</button>
    </form>
  </div>

  <script>
    const isNewSel = document.getElementById('isNew');
    const platformContainer = document.getElementById('platformContainer');
    const platformSel = document.getElementById('platform');
    const productGroupContainer = document.getElementById('productGroupContainer');
    const productGroupSel = document.getElementById('productGroup');

    const productGroups = {
      'Platform A': ['Group A1', 'Group A2'],
      'Platform B': ['Group B1', 'Group B2'],
      'Platform C': ['Group C1', 'Group C2']
    };

    isNewSel.addEventListener('change', () => {
      if (isNewSel.value === 'Yes') {
        platformContainer.classList.remove('hidden');
        platformSel.required = true;
      } else {
        platformContainer.classList.add('hidden');
        platformSel.required = false;
        platformSel.value = '';
        productGroupContainer.classList.add('hidden');
        productGroupSel.required = false;
        productGroupSel.value = '';
        productGroupSel.innerHTML = '<option value="">Select product group</option>';
      }
    });

    platformSel.addEventListener('change', () => {
      const list = productGroups[platformSel.value] || [];
      productGroupSel.innerHTML = '<option value="">Select product group</option>';
      list.forEach(g => productGroupSel.add(new Option(g, g)));
      if (list.length) {
        productGroupContainer.classList.remove('hidden');
        productGroupSel.required = true;
      } else {
        productGroupContainer.classList.add('hidden');
        productGroupSel.required = false;
      }
    });

    // Confluence: Autofill current user email
    if (typeof AP !== 'undefined' && AP.user) {
      AP.user.getCurrentUser(function(user) {
        document.getElementById('emailField').value = user.email;
      });
    } else {
      document.getElementById('emailField').value = 'user@example.com'; // fallback
    }
  </script>
</body>
</html>
