<!DOCTYPE html><html>
<head>
  <title>Service Request Form</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 30px;
    }
    .form-section {
      max-width: 700px;
      margin: auto;
      border: 1px solid #ccc;
      padding: 20px;
      border-radius: 8px;
      background-color: #f9f9f9;
    }
    h2 {
      text-align: center;
    }
    .form-group {
      margin-bottom: 15px;
    }
    label {
      display: block;
      margin-bottom: 5px;
      font-weight: bold;
    }
    input, select, textarea {
      width: 100%;
      padding: 8px;
      box-sizing: border-box;
    }
    .required::after {
      content: " *";
      color: red;
    }
    button {
      padding: 10px 20px;
      font-size: 16px;
      cursor: pointer;
    }
  </style>
</head>
<body><div class="form-section">
  <h2>Service Request Form</h2>
  <form id="jiraForm"><div class="form-group">
  <label class="required">Request Classification</label>
  <select id="requestClassification">
    <option>-- Select --</option>
    <option>Classification A</option>
    <option>Classification B</option>
  </select>
</div>

<div class="form-group">
  <label class="required">Request Type</label>
  <select id="requestType">
    <option>-- Select --</option>
    <option>Type A</option>
    <option>Type B</option>
  </select>
</div>

<div class="form-group">
  <label class="required">Is the project part of NEW</label>
  <select id="isNEW">
    <option value="no">No</option>
    <option value="yes">Yes</option>
  </select>
</div>

<div class="form-group">
  <label class="required">Environment</label>
  <select id="environment">
    <option>-- Select --</option>
    <option>DEV</option>
    <option>QA</option>
    <option>PROD</option>
  </select>
</div>

<div class="form-group">
  <label class="required">Application Name</label>
  <input type="text" id="applicationName" />
</div>

<div class="form-group">
  <label class="required">MALCODE</label>
  <input type="text" id="malcode" placeholder="Put 'NA' if unknown" />
</div>

<div class="form-group">
  <label class="required">Test Set</label>
  <input type="text" id="testSet" placeholder="Put 'NA' if not applicable" />
</div>

<div class="form-group">
  <label>Version</label>
  <input type="text" id="version" />
</div>

<div class="form-group">
  <label class="required">Email Address</label>
  <input type="email" id="email" value="SaiSreekarReddy.Yammanuru@td.com" />
</div>

<div class="form-group">
  <label class="required">Target Start Date</label>
  <input type="date" id="startDate" />
</div>

<div class="form-group">
  <label class="required">Target End Date</label>
  <input type="date" id="endDate" />
</div>

<div class="form-group">
  <label class="required">Request Summary</label>
  <input type="text" id="summary" placeholder="JIRA Notification Email subject (include Test Set)" />
</div>

<div class="form-group">
  <label class="required">Request Details</label>
  <textarea id="details" rows="5" placeholder="Enter issue details and Error info or Stack"></textarea>
</div>

<div class="form-group" style="text-align: center;">
  <button type="submit">Submit</button>
</div>

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