project = YOURPROJECTKEY AND (
  (assignee = a AND created >= "2025-01-01" AND created <= "2025-01-30") OR
  (assignee = b AND created >= "2025-07-05" AND created <= "2025-07-27") OR
  (assignee = c AND created >= "2025-05-12" AND created <= "2025-05-31")
)
ORDER BY created ASC