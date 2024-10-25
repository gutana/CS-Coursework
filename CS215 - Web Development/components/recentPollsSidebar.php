<?php

Database::Init();

$recentPolls = Database::Query("SELECT poll_id, question 
                                FROM polls
                                WHERE start_time < CURTIME() AND end_time > CURTIME()
                                ORDER BY start_time DESC
                                LIMIT 5;");

Database::Close();

?>

<h4>
    RECENT POLLS
</h4>
<div class="recent-polls">
    
<?php 

while ($row = $recentPolls->getRow())
{
    $poll = new MinimalPoll($row['poll_id'], $row['question']);
}

?>

</div>