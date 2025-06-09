const http = require('http');
const fs = require('fs');
const path = require('path');

const JIRA_BASE_URL = process.env.JIRA_BASE_URL;
const JIRA_USER = process.env.JIRA_USER;
const JIRA_TOKEN = process.env.JIRA_TOKEN;
const JIRA_PROJECT_KEY = process.env.JIRA_PROJECT_KEY;

const SMTP_HOST = process.env.SMTP_HOST;
const SMTP_PORT = parseInt(process.env.SMTP_PORT || '587', 10);
const SMTP_USER = process.env.SMTP_USER;
const SMTP_PASS = process.env.SMTP_PASS;
const EMAIL_RECIPIENTS = (process.env.EMAIL_RECIPIENTS || '').split(',').filter(Boolean);

async function createJiraTicket(data) {
  if (!JIRA_BASE_URL) {
    return 'DEMO-0001';
  }
  const url = `${JIRA_BASE_URL}/rest/api/2/issue`;
  const payload = {
    fields: {
      project: { key: JIRA_PROJECT_KEY },
      summary: data.summary || 'No summary',
      description: data.description || '',
      issuetype: { name: 'Task' },
    },
  };
  const headers = { 'Content-Type': 'application/json' };
  if (JIRA_USER && JIRA_TOKEN) {
    const auth = Buffer.from(`${JIRA_USER}:${JIRA_TOKEN}`).toString('base64');
    headers['Authorization'] = `Basic ${auth}`;
  }
  const response = await fetch(url, {
    method: 'POST',
    headers,
    body: JSON.stringify(payload),
  });
  if (!response.ok) {
    const text = await response.text();
    throw new Error(`JIRA error ${response.status}: ${text}`);
  }
  const json = await response.json();
  return json.key;
}

async function sendEmail(ticketKey, data) {
  if (!SMTP_HOST || EMAIL_RECIPIENTS.length === 0) {
    return;
  }
  try {
    const nodemailer = require('nodemailer');
    const transporter = nodemailer.createTransport({
      host: SMTP_HOST,
      port: SMTP_PORT,
      secure: false,
      auth: SMTP_USER ? { user: SMTP_USER, pass: SMTP_PASS } : undefined,
    });
    const body = Object.entries(data)
      .map(([k, v]) => `${k}: ${v}`)
      .join('\n');
    await transporter.sendMail({
      from: SMTP_USER,
      to: EMAIL_RECIPIENTS.join(', '),
      subject: `New ticket created: ${ticketKey}`,
      text: `Ticket: ${ticketKey}\n${body}`,
    });
  } catch (err) {
    console.error('Failed to send email (nodemailer not installed?)', err);
  }
}

function parseForm(req) {
  return new Promise((resolve, reject) => {
    let body = '';
    req.on('data', chunk => {
      body += chunk;
    });
    req.on('end', () => {
      const params = new URLSearchParams(body);
      const data = {};
      for (const [key, value] of params.entries()) {
        data[key] = value;
      }
      resolve(data);
    });
    req.on('error', reject);
  });
}

const server = http.createServer(async (req, res) => {
  if (req.method === 'GET' && req.url === '/') {
    const filePath = path.join(__dirname, 'templates', 'index.html');
    fs.readFile(filePath, (err, data) => {
      if (err) {
        res.writeHead(500);
        res.end('Error loading page');
        return;
      }
      res.writeHead(200, { 'Content-Type': 'text/html' });
      res.end(data);
    });
    return;
  }
  if (req.method === 'POST' && req.url === '/submit') {
    try {
      const formData = await parseForm(req);
      const ticketKey = await createJiraTicket(formData);
      await sendEmail(ticketKey, formData);
      res.writeHead(200, { 'Content-Type': 'text/plain' });
      res.end(`Created ticket ${ticketKey} and sent email`);
    } catch (err) {
      console.error(err);
      res.writeHead(500, { 'Content-Type': 'text/plain' });
      res.end('An error occurred');
    }
    return;
  }
  res.writeHead(404);
  res.end('Not Found');
});

const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`);
});
