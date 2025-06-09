import java.io.OutputStream;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import javax.net.ssl.*;
import java.security.cert.X509Certificate;
import java.net.HttpURLConnection;

public class JiraSubtaskCreator {

    public static void main(String[] args) throws Exception {
        if (args.length < 2) {
            System.out.println("Usage: JiraSubtaskCreator <PARENT_TICKET> <ASSIGNEE>");
            return;
        }

        String parentTicket = args[0];
        String assignee = args[1];

        String jiraDomain = "your-domain.atlassian.net"; // <-- Change this
        String jiraToken = "your_actual_pat_here";       // <-- Change this
        String jiraUrl = "https://" + jiraDomain + "/rest/api/2/issue";
        String authHeader = "Bearer " + jiraToken;

        // Disable SSL certificate validation (for testing only!)
        TrustManager[] trustAllCerts = new TrustManager[] {
            new X509TrustManager() {
                public X509Certificate[] getAcceptedIssuers() { return null; }
                public void checkClientTrusted(X509Certificate[] certs, String authType) {}
                public void checkServerTrusted(X509Certificate[] certs, String authType) {}
            }
        };
        SSLContext sc = SSLContext.getInstance("SSL");
        sc.init(null, trustAllCerts, new java.security.SecureRandom());
        HttpsURLConnection.setDefaultSSLSocketFactory(sc.getSocketFactory());
        HttpsURLConnection.setDefaultHostnameVerifier((hostname, session) -> true);

        // Use subtask type ID and transition ID as needed
        String issueTypeId = "10002"; // sub-task
        String transitionId = "31";   // Done

        String jsonPayload = "{\"fields\":{"
            + "\"project\":{\"key\":\"" + parentTicket.split("-")[0] + "\"},"
            + "\"parent\":{\"key\":\"" + parentTicket + "\"},"
            + "\"summary\":\"Subtask from Java\","
            + "\"description\":\"Created via Java program on z/OS\","
            + "\"issuetype\":{\"id\":\"" + issueTypeId + "\"},"
            + "\"assignee\":{\"name\":\"" + assignee + "\"}"
            + "}}";

        URL url = new URL(jiraUrl);
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setRequestMethod("POST");
        conn.setRequestProperty("Authorization", authHeader);
        conn.setRequestProperty("Content-Type", "application/json");
        conn.setDoOutput(true);

        try (OutputStream os = conn.getOutputStream()) {
            byte[] input = jsonPayload.getBytes(StandardCharsets.UTF_8);
            os.write(input, 0, input.length);
        }

        int responseCode = conn.getResponseCode();
        System.out.println("HTTP Response Code: " + responseCode);
        if (responseCode == 201) {
            System.out.println("✅ Subtask created successfully.");
        } else {
            System.out.println("❌ Failed to create subtask. Check credentials and parameters.");
        }
    }
}
