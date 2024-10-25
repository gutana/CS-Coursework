let customStartDateChosen = false; 
let customEndDateChosen = false; 

function attachEventListeners() {
    document.getElementById("start-option").addEventListener("change", checkStartTimeDropdown);
    document.getElementById("end-option").addEventListener("change", checkEndTimeDropdown);
    document.getElementById("poll-submit-btn").addEventListener("click", verifyPoll);
}

function checkStartTimeDropdown(event){
    let choice = event.target.options.selectedIndex;
    let customDateInput = document.getElementById("start-time");
    // 0 = start immediately
    // 1 = custom start time 
    if (choice === 1)
    {
        customDateInput.style.opacity = "100%";
        customStartDateChosen = true; 
    } else {
        customDateInput.style.opacity = "0%";
        customStartDateChosen = false; 
    }
}

function checkEndTimeDropdown(event){
    let choice = event.target.options.selectedIndex;
    let customDateInput = document.getElementById("end-time");
    // 0 = 1 day
    // 1 = 1 week 
    // 2 = 1 month 
    // 3 = custom start time 
    if (choice === 3)
    {
        customDateInput.style.opacity = "100%";
        customEndDateChosen = true; 
    } else {
        customDateInput.style.opacity = "0%";
        customEndDateChosen = false; 
    }
}

function verifyPoll(event) {
    let inputElemArray = document.getElementsByClassName("poll-input");
    let questionInput = inputElemArray[0];

    let questionValid;

    if (questionInput.value == "") {
        questionValid = false; 
    } 
    else {   
        questionValid = Validator.verifyLength(questionInput.value, 100, "maximum")
    }

    let answersValid = true;
    let answersCount = 0;
    
    // check length for each of the answers
    for (let i = 1; i < inputElemArray.length && answersValid; i++)
    {
        let val = inputElemArray[i].value;
        if (val != "")
        {
            answersValid = Validator.verifyLength(val, 50, "maximum");
            answersCount++;
        }
    }

    let errText = document.getElementById("poll-error-text");
    errText.innerHTML = ""; // reset so we don't have left-over text

    let customDatesNotEmpty = true; 

    if (customStartDateChosen)
    {
        let dateElem = document.getElementById("start-time");
        if (dateElem.value == "")
        {
            customDatesNotEmpty = false; 
        }
    }
    if (customEndDateChosen)
    {
        let dateElem = document.getElementById("end-time");
        if (dateElem.value == "")
        {
            customDatesNotEmpty = false; 
        }
    }

    let shouldSubmit = true; 

    if (!customDatesNotEmpty) {
        errText.innerHTML += "- Custom start/end times must be valid.";
        errText.innerHTML += "<br />";
        shouldSubmit = false; 
    }
    if (!questionValid)
    {
        errText.innerHTML += "- Question cannot be empty or exceed 100 characters.";
        errText.innerHTML += "<br />";
        shouldSubmit = false; 
    }
    if (!answersValid)
    {
        errText.innerHTML += "- Answers cannot exceed 50 characters.";
        errText.innerHTML += "<br />";
        shouldSubmit = false; 
    }
    if (answersCount < 2)
    {
        errText.innerHTML += "- Polls must have at least two answers.";
        errText.innerHTML += "<br />";
        shouldSubmit = false; 
    }

    if (!shouldSubmit) {
        event.preventDefault();
    }
}

attachEventListeners();