<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8" />
    <title>Service Request Form</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background: #f9f9f9;
            padding: 20px;
        }

        form {
            max-width: 700px;
            margin: auto;
            background: #fff;
            padding: 30px;
            border-radius: 6px;
            border: 1px solid #ccc;
            /* Thin border */
            box-shadow: none;
            /* Remove previous shadow */
        }



        h2 {
            margin-bottom: 20px;
            font-weight: normal;
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

        input[type="text"],
        input[type="email"],
        input[type="date"],
        select,
        textarea {
            width: 100%;
            padding: 10px;
            font-size: 16px;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
        }

        textarea {
            resize: vertical;
            height: 100px;
        }

        .hint {
            font-size: 12px;
            color: #666;
            margin-top: 4px;
        }

        button {
            padding: 12px 20px;
            font-size: 16px;
            background-color: #0078d7;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            margin-top: 20px;
        }

        button:hover {
            background-color: #005fa3;
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

    <form id="serviceForm" novalidate>
        <h2>Service Request Form</h2>

        <div class="form-group">
            <label class="required">Request Classification</label>
            <select id="classification" required>
                <option value="">Select classification</option>
                <option>Bug</option>
                <option>Enhancement</option>
                <option>New Feature</option>
            </select>
        </div>

        <div class="form-group">
            <label class="required">Request Type</label>
            <select id="requestType" required>
                <option value="">Select request type</option>
                <option>Standard</option>
                <option>Urgent</option>
            </select>
        </div>

        <h3 style="margin: 20px 0 5px; font-weight: bold;">Next Evolution of Work</h3>
        <hr style="border: none; border-top: 1px solid #ddd; margin: 5px 0 20px;" />

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
            <select id="environment" required>
                <option value="">Select environment</option>
                <option>DEV</option>
                <option>QA</option>
                <option>UAT</option>
                <option>PROD</option>
            </select>
        </div>

        <div class="form-group">
            <label class="required">Application Name</label>
            <input type="text" id="appName" required placeholder="Enter application name">
        </div>

        <div class="form-group">
            <label class="required">MALCODE</label>
            <input type="text" id="malcode" required placeholder="Enter MALCODE">
            <div class="hint">Please put 'NA' if malcode is unknown</div>
        </div>

        <div class="form-group">
            <label class="required">Test Set</label>
            <input type="text" id="testSet" required placeholder="Enter test set">
            <div class="hint">Please input 'NA' if test set not applicable</div>
        </div>

        <div class="form-group">
            <label>Version</label>
            <input type="text" id="version" placeholder="Enter version">
        </div>

        <div class="form-group">
            <label class="required">Email Address</label>
            <input type="email" id="email" required value="SaiSreekarReddy.Yammanuru@td.com">
        </div>

        <div class="form-group">
            <label class="required">Target Start Date</label>
            <input type="date" id="startDate" required>
        </div>

        <div class="form-group">
            <label class="required">Target End Date</label>
            <input type="date" id="endDate" required>
        </div>

        <div class="form-group">
            <label class="required">Request Summary</label>
            <input type="text" id="summary" required placeholder="JIRA Notification Email subject (include Test Set)">
        </div>

        <div class="form-group">
            <label class="required">Request Details</label>
            <textarea id="details" required
                placeholder="Please enter issue details and Error info or Error Stack"></textarea>
        </div>

        <button type="submit">Submit</button>
    </form>

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

        document.getElementById('serviceForm').addEventListener('submit', function (e) {
            e.preventDefault();
            let valid = true;
            const requiredFields = this.querySelectorAll('[required]');
            requiredFields.forEach(el => {
                if (!el.value) {
                    el.classList.add('error');
                    valid = false;
                } else {
                    el.classList.remove('error');
                }
            });
            const sd = document.getElementById('startDate').valueAsDate;
            const ed = document.getElementById('endDate').valueAsDate;
            if (sd && ed && ed < sd) {
                alert('End date must be on or after start date.');
                valid = false;
            }
            if (valid) {
                alert('Form submitted successfully!');
                this.reset();
                platformContainer.classList.add('hidden');
                productGroupContainer.classList.add('hidden');
            } else {
                alert('Please fill in all required fields correctly.');
            }
        });
    </script>

</body>

</html>
