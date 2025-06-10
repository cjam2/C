<!DOCTYPE html><html>
<head>
  <title>Service Request Form</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 30px;
    }
    label {
      display: block;
      margin-top: 10px;
      font-weight: bold;
    }
    input, select, textarea {
      width: 100%;
      padding: 6px;
      margin-top: 4px;
    }
    .hidden {
      display: none;
    }
    .required::after {
      content: " *";
      color: red;
    }
    .form-section {
      max-width: 600px;
    }
    button {
      margin-top: 20px;
      padding: 10px 20px;
    }
  </style>
</head>
<body><div class="form-section">
  <h2>Service Request Form</h2>
  <form id="jiraForm">
    <label class="required">Request Classification
      <select id="requestClassification">
        <option>-- Select --</option>
        <option>Classification A</option>
        <option>Classification B</option>
      </select>
    </label><label class="required">Request Type
  <select id="requestType">
    <option>-- Select --</option>
    <option>Type A</option>
    <option>Type B</option>
  </select>
</label>

<label class="required">Is the project part of NEW
  <select id="isNEW">
    <option value="no">No</option>
    <option value="yes">Yes</option>
  </select>
</label>

<label class="required">Environment
  <select id="environment">
    <option>-- Select --</option>
    <option>DEV</option>
    <option>QA</option>
    <option>PROD</option>
  </select>
</label>

<label class="required">Application Name
  <input type="text" id="applicationName" />
</label>

<label class="required">MALCODE
  <input type="text" id="malcode" placeholder="Put 'NA' if unknown" />
</label>

<label class="required">Test Set
  <input type="text" id="testSet" placeholder="Put 'NA' if not applicable" />
</label>

<label>Version
  <input type="text" id="version" />
</label>

<label class="required">Email Address
  <input type="email" id="email" value="SaiSreekarReddy.Yammanuru@td.com" />
</label>

<label class="required">Target Start Date
  <input type="date" id="startDate" />
</label>

<label class="required">Target End Date
  <input type="date" id="endDate" />
</label>

<label class="required">Request Summary
  <input type="text" id="summary" placeholder="JIRA Notification Email subject (include Test Set)" />
</label>

<label class="required">Request Details
  <textarea id="details" rows="5" placeholder="Enter issue details and Error info or Stack"></textarea>
</label>

<button type="submit">Submit</button>

  </form>
</div><script>
  document.getElementById('jiraForm').addEventListener('submit', function(e) {
    e.preventDefault();
    const data = {
      requestClassification: document.getElementById('requestClassification').value,
      requestType: document.getElementById('requestType').value,
      isNEW: document.getElementById('isNEW').value,
      environment: document.getElementById('environment').value,
      applicationName: document.getElementById('applicationName').value,
      malcode: document.getElementById('malcode').value,
      testSet: document.getElementById('testSet').value,
      version: document.getElementById('version').value,
      email: document.getElementById('email').value,
      startDate: document.getElementById('startDate').value,
      endDate: document.getElementById('endDate').value,
      summary: document.getElementById('summary').value,
      details: document.getElementById('details').value
    };

    console.log("Form Data Submitted:", data);

    // TODO: Add Jira Ticket API integration here
    // TODO: Add Email sending logic here (via server backend)

    alert("Form submitted. Jira ticket will be created and email sent.");
  });
</script></body>
</html>