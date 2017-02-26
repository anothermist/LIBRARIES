var app = angular.module('app', ['ng-admin']);
app.config(['NgAdminConfigurationProvider', function (nga) {
    // create an admin application
    var admin = nga.application('Blynk User Administration', false)
        .baseApiUrl(location.protocol + '//' + window.location.hostname + (location.port == 80 ? '' : (':' + location.port)) + location.pathname + '/'); // main API endpoint


    var customHeaderTemplate =
    '<div class="navbar-header">' +
        '<a class="navbar-brand" href="#" ng-click="appController.displayHome()">Blynk Administration</a>' +
    '</div>' +
    '<p class="navbar-text navbar-right">' +
        '<a href="javascript:post(&quot;/business/logout&quot;)">' +
            '<span class="glyphicon glyphicon-log-out"></span>&nbsp;Logout' +
        '</a>' +
    '</p>';



    admin.header(customHeaderTemplate);

    // create a user entity
    // the API endpoint for this entity will be 'http://jsonplaceholder.typicode.com/users/:id
    var userInfo = nga.entity('projects').identifier(nga.field('id'));
    // set the fields of the user entity list view
    userInfo.listView()
        .sortField('updatedAt')
        .fields([
            nga.field('name').isDetailLink(true).map(
                function truncate(value, entry) {
                    return !value ? "(No Name)" : value;
                }
            ),
            nga.field('token'),
            nga.field('updatedAt', 'datetime'),
            nga.field('metadata', 'json')
        ])
        .filters([
            nga.field('name').label('').pinned(true)
                .template('<div class="input-group"><input type="text" ng-model="value" placeholder="Search" class="form-control"></input><span class="input-group-addon"><i class="glyphicon glyphicon-search"></i></span></div>')
        ]);

    userInfo.creationView().fields([
        nga.field('name'),
        nga.field('boardType', 'choice')
            .choices([
                {value: 'Arduino 101', label: 'Arduino 101'},
                {value: 'Arduino Due', label: 'Arduino Due'},
                {value: 'Arduino Leonardo', label: 'Arduino Leonardo'},
                {value: 'Arduino Mega', label: 'Arduino Mega'},
                {value: 'Arduino Micro', label: 'Arduino Micro'},
                {value: 'Arduino Mini', label: 'Arduino Mini'},
                {value: 'Arduino MKR1000', label: 'Arduino MKR1000'},
                {value: 'Arduino Nano', label: 'Arduino Nano'},
                {value: 'Arduino Pro Micro', label: 'Arduino Pro Micro'},
                {value: 'Arduino Pro Mini', label: 'Arduino Pro Mini'},
                {value: 'Arduino UNO', label: 'Arduino UNO'},
                {value: 'Arduino Yun', label: 'Arduino Yun'},
                {value: 'Arduino Zero', label: 'Arduino Zero'},
                {value: 'ESP8266', label: 'ESP8266'},
                {value: 'Generic Board', label: 'Generic Board'},
                {value: 'Intel Edison', label: 'Intel Edison'},
                {value: 'Intel Galileo', label: 'Intel Galileo'},
                {value: 'LinkIt ONE', label: 'LinkIt ONE'},
                {value: 'Microduino Core+', label: 'Microduino Core+'},
                {value: 'Microduino Core', label: 'Microduino Core'},
                {value: 'Microduino CoreRF', label: 'Microduino CoreRF'},
                {value: 'Microduino CoreUSB', label: 'Microduino CoreUSB'},
                {value: 'NodeMCU', label: 'NodeMCU'},
                {value: 'Particle Core', label: 'Particle Core'},
                {value: 'Particle Electron', label: 'Particle Electron'},
                {value: 'Particle Photon', label: 'Particle Photon'},
                {value: 'Raspberry Pi 3 B', label: 'Raspberry Pi 3 B'},
                {value: 'Raspberry Pi 2/A+/B+', label: 'Raspberry Pi 2/A+/B+'},
                {value: 'Raspberry Pi B (Rev1)', label: 'Raspberry Pi B (Rev1)'},
                {value: 'Raspberry Pi A/B (Rev2)', label: 'Raspberry Pi A/B (Rev2)'},
                {value: 'RedBearLab CC3200/Mini', label: 'RedBearLab CC3200/Mini'},
                {value: 'Seeed Wio Link', label: 'Seeed Wio Link'},
                {value: 'SparkFun Blynk Board', label: 'SparkFun Blynk Board'},
                {value: 'SparkFun ESP8266 Thing', label: 'SparkFun ESP8266 Thing'},
                {value: 'SparkFun Photon RedBoard', label: 'SparkFun Photon RedBoard'},
                {value: 'TI CC3200-LaunchXL', label: 'TI CC3200-LaunchXL'},
                {value: 'TI Tiva C Connected', label: 'TI Tiva C Connected'},
                {value: 'TinyDuino', label: 'TinyDuino'},
                {value: 'WeMos D1', label: 'WeMos D1'},
                {value: 'WeMos D1 mini', label: 'WeMos D1 mini'},
                {value: 'Wildfire v2', label: 'Wildfire v2'},
                {value: 'Wildfire v3', label: 'Wildfire v3'},
                {value: 'Wildfire v4', label: 'Wildfire v4'},
                {value: 'WiPy', label: 'WiPy'}
            ]),
        nga.field('metadata', 'json')
    ]);

    userInfo.editionView()
        .title('Edit project "{{entry.values.name}}"')
        .fields(
            nga.field('id'),
            nga.field('name'),
            nga.field('createdAt'),
            nga.field('updatedAt'),
            nga.field('token'),
            nga.field('theme', 'choice')
                .choices([
                    {value: 'Blynk', label: 'Blynk'},
                    {value: 'SparkFun', label: 'SparkFun'}
                ]),
            nga.field('boardType', 'choice')
                        .choices([
                            {value: 'Arduino Due', label: 'Arduino Due'},
                            {value: 'Arduino Leonardo', label: 'Arduino Leonardo'},
                            {value: 'Arduino Mega', label: 'Arduino Mega'},
                            {value: 'Arduino Micro', label: 'Arduino Micro'},
                            {value: 'Arduino Mini', label: 'Arduino Mini'},
                            {value: 'Arduino Nano', label: 'Arduino Nano'},
                            {value: 'Arduino Pro Micro', label: 'Arduino Pro Micro'},
                            {value: 'Arduino Pro Mini', label: 'Arduino Pro Mini'},
                            {value: 'Arduino UNO', label: 'Arduino UNO'},
                            {value: 'Arduino Yun', label: 'Arduino Yun'},
                            {value: 'ESP8266', label: 'ESP8266'},
                            {value: 'Generic Board', label: 'Generic Board'},
                            {value: 'Intel Edison', label: 'Intel Edison'},
                            {value: 'Intel Galileo', label: 'Intel Galileo'},
                            {value: 'LinkIt ONE', label: 'LinkIt ONE'},
                            {value: 'NodeMCU', label: 'NodeMCU'},
                            {value: 'Particle Core', label: 'Particle Core'},
                            {value: 'Particle Photon', label: 'Particle Photon'},
                            {value: 'Raspberry Pi 2/A+/B+', label: 'Raspberry Pi 2/A+/B+'},
                            {value: 'Raspberry Pi B (Rev1)', label: 'Raspberry Pi B (Rev1)'},
                            {value: 'Raspberry Pi A/B (Rev2)', label: 'Raspberry Pi A/B (Rev2)'},
                            {value: 'RedBearLab CC3200/Mini', label: 'RedBearLab CC3200/Mini'},
                            {value: 'SparkFun Blynk Board', label: 'SparkFun Blynk Board'},
                            {value: 'SparkFun ESP8266 Thing', label: 'SparkFun ESP8266 Thing'},
                            {value: 'SparkFun Photon RedBoard', label: 'SparkFun Photon RedBoard'},
                            {value: 'TI CC3200-LaunchXL', label: 'TI CC3200-LaunchXL'},
                            {value: 'TI Tiva C Connected', label: 'TI Tiva C Connected'},
                            {value: 'TinyDuino', label: 'TinyDuino'},
                            {value: 'WeMos D1', label: 'WeMos D1'},
                            {value: 'WeMos D1 mini', label: 'WeMos D1 mini'},
                            {value: 'Wildfire v2', label: 'Wildfire v2'},
                            {value: 'Wildfire v3', label: 'Wildfire v3'},
                            {value: 'WiPy', label: 'WiPy'}
                        ]),
            nga.field('metadata', 'json')
        );

    // customize menu
    admin.menu(nga.menu().autoClose(false)
            .addChild(nga.menu(userInfo).icon('<span class="glyphicon glyphicon-user"></span>'))
    );


    // add the user entity to the admin application
    admin.addEntity(userInfo);

    // attach the admin application to the DOM and execute it
    nga.configure(admin);
}]);

function post(action) {
    var form = document.createElement("form");
    form.setAttribute("method", "POST");
    form.setAttribute("action", action);
    document.body.appendChild(form);
    form.submit();
}