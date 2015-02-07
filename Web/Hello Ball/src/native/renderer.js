'use strict';

var renderDisplayList = function (svgRootNode, displayList) {
    displayList.forEach(function (displayElement) {
        var node = svgRootNode.getElementById("ball" + displayElement.index);
        if (!node) {
            node = document.createElementNS("http://www.w3.org/2000/svg", "circle");
            node.id = 'ball' + displayElement.index;

            node.setAttributeNS(null, 'fill', '#' + displayElement.color.toString(16));
            node.setAttributeNS(null, 'stroke-width', '3px');
            node.setAttributeNS(null, 'stroke-dasharray', '10,5');
            svgRootNode.appendChild(node);
        }
        node.setAttributeNS(null, 'r', displayElement.radius);
        node.setAttributeNS(null, 'cx', displayElement.x);
        node.setAttributeNS(null, 'cy', displayElement.y);
        if (displayElement.hits.length) {
            node.setAttributeNS(null, 'stroke', 'red');
        } else {
            node.setAttributeNS(null, 'stroke', 'none');
        }
    });

};
