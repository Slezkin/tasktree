# tasktree

Структура БД (вариант с работы):

Таблица "tasks":

поле				тип		отношение	авт.	обяз. при созд.	обяз.+ не авт.

id				INT		-		+	+		-
name				VARCHAR(32)	-		-	+		+
creation_date			DATE		-		+	+		-
start_date			DATE		-		-	-		-
is_open				INT		-		-	+		+
is_active			INT		-		-	+		+
is_hierarcy_end			INT		-		+	+		-
level				INT		-		+	+		-
parent_task_id			INT		tasks		-	+		-
anticipating_task_id		INT		tasks		-	-		-
source_subject_id		INT		subjects	-	-		-
blocking_subject_id		INT		subjects	-	-		-
blocking_start_date		DATE		-		-	-		-
blocking_end_date		DATE		-		-	-		-
descr				TEXT		-		-	-		-
descr_files			BLOB		-		-	-		-
result				TEXT		-		-	-		-
result_fules			BLOB		-		-	-		-
finish_date			DATE		-		-	-		-

Таблица "subjects":

id		INT
name		VARCHAR(64)
upper_id	INT
is_internal	INT

Таблица "records":

id		INT
task_id		INT
works		TEXT
date		DATE
hours		INT


