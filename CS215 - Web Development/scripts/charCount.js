const MAX_QUESTION = 100;
const MAX_ANSWER   = 50; 

function init() {
    let inputElemArray = document.getElementsByClassName("poll-input");

    for (let i = 0; i < 6; i++) { // This could be more dynamic, but we know there's only 6 elements in this array. 
        inputElemArray[i].addEventListener('input', textChanged);
    }
}

function textChanged(event) {
    let numberText = event.target.nextSibling;
    let length = event.target.value.length;



    if (event.target.classList.contains("poll-question-input")) {
        setMaxCharStyle(numberText, length, MAX_QUESTION);
        numberText.innerHTML = MAX_QUESTION - length;
    } else {
        setMaxCharStyle(numberText, length, MAX_ANSWER);
        numberText.innerHTML = MAX_ANSWER - length;
    }
}

function setMaxCharStyle(elem, currentLength, maxLenth) {
    let difference = maxLenth - currentLength; 

    if (difference < 0) {
        elem.style.borderColor = "red";
        elem.style.color = "#FFFFFF";
        elem.style.backgroundColor = "red";
    } else if (difference == 0) {
        elem.style.borderColor = "red";
        elem.style.color = "black";
        elem.style.backgroundColor = "#f6a90f";
    } else if (difference < 11) {
        elem.style.borderColor = "#DD4D38";
        elem.style.color = "black";
        elem.style.backgroundColor = "#ffde1e";
    } else {
        elem.style.borderColor = "#2D89FC";
        elem.style.color = "black";
        elem.style.backgroundColor = "transparent";
    }
}

window.onload = () => {
    init();
}