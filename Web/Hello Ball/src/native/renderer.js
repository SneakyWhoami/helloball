'use strict';

var int2color = function (p_color) {
    var hexStr = (+p_color).toString(16);
    return '#' + '000000'.substring(0, 6 - hexStr.length) + hexStr;
};

var phase2color = function (p_phase) {
    var tone = Math.round(255 * p_phase).toString(16);
    if (tone.length < 2) {
        tone = '0' + tone;
    }
    return '#' + tone + tone + tone;
};

var renderDisplayList = function (svgRootNode, displayList) {
    var displayElement;
    for (var index in displayList) {
        displayElement = displayList[index];
        var node = svgRootNode.getElementById("ball" + index);
        if (!node) {
            node = document.createElementNS("http://www.w3.org/2000/svg", "circle");
            node.id = 'ball' + index;
            node.setAttributeNS(null, 'stroke-width', '3px');
            node.setAttributeNS(null, 'stroke-dasharray', '10,5');
            svgRootNode.appendChild(node);
        }
        node.setAttributeNS(null, 'r', displayElement.radius);
        node.setAttributeNS(null, 'cx', displayElement.x);
        node.setAttributeNS(null, 'cy', displayElement.y);
        node.setAttributeNS(null, 'fill', int2color(displayElement.color));
    };
};

var renderEps = function (svgRootNode, eps) {
    var node = svgRootNode.getElementById("eps");
    if (!node) {
        node = document.createElementNS("http://www.w3.org/2000/svg", "text");
        node.id = "eps";
        node.setAttributeNS(null,"x", 12);
        node.setAttributeNS(null,"y", 16);
        node.setAttributeNS(null,"font-size","18");
        var textNode = document.createTextNode("EPS: " + Math.round(eps));
        node.appendChild(textNode);
        svgRootNode.appendChild(node);
    } else {
        node.textContent = "EPS: " + Math.round(eps);
    }
};

var renderPhase = function (svgRootNode, phase) {
    svgRootNode.setAttributeNS(null, "style", 'background-color:'+phase2color(phase));
};