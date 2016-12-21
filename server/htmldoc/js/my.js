goog.require('proto.runeio.proto.character.game_state');
goog.require('proto.runeio.proto.character.user_input');
goog.require('proto.runeio.proto.character.user_input_codes_base');
goog.require('proto.runeio.proto.info');
goog.require('proto.runeio.proto.message');
goog.require('proto.runeio.proto.user.user');

goog.require('goog.structs');
goog.require('goog.structs.Map');
goog.require('goog.net.WebSocket');
goog.require('goog.events');

var runeio = (function () {
    var canvas = undefined;
    var game = undefined;
    var ws_control = undefined;

    return {
        init: function (canvasElementId) {
            canvas = new Canvas(canvasElementId);
            game = new Game(canvas);

            ws_control = new WebSocketControl("ws://ws." + window.location.hostname + "/", false);
            ws_control.listen(game.parse_message, game);
            game.set_send_func(goog.bind(ws_control.send, ws_control));
            ws_control.open();
        }
    }
}());

/* Player class */
function Player(id) {
    this.id = id || undefined;
    this.x = 0;
    this.y = 0;
    this.vx = 0;
    this.vy = 0;
    this.r = 20;
    this.angle = 0;

    this.last_input_id = 0;
    this.last_server_parsed_input_id = -1;

    this.is_updated = false;
}

Player.prototype.draw = function (ctx) {
    Player.circle(ctx, this.x, this.y, this.r, this.angle);
}

Player.prototype.parse_net_pub = function (net_msg) {
    var net_position = net_msg.getPosition();
    var net_point = net_position.getPoint();
    var net_velocity = net_position.getVelocity();

    this.x = net_point.getX();
    this.y = net_point.getY()
    this.vx = net_velocity.getX();
    this.vy = net_velocity.getY()
    this.angle = net_position.getViewAngle();

    this.is_updated = true;

    return this;
}

Player.prototype.parse_net_priv = function (net_msg) {
    this.last_server_parsed_input_id = net_msg.getLastParsedInputId();

    return this;
}

Player.parse_net_pub = function (id, net_msg) {
    return (new Player(id)).parse_net_pub(net_msg);
}

Player.circle = function (ctx, x, y, r, a) {
    var s = a - 0.05 * Math.PI;
    var e = a + 0.05 * Math.PI;

    ctx.beginPath();
    ctx.arc(x, y, r, e, s);

    ctx.moveTo(x + r * Math.cos(s), y + r * Math.sin(s));
    ctx.lineTo(x + 1.5 * r * Math.cos(a), y + 1.5 * r * Math.sin(a));

    ctx.moveTo(x + r * Math.cos(e), y + r * Math.sin(e));
    ctx.lineTo(x + 1.5 * r * Math.cos(a), y + 1.5 * r * Math.sin(a));
    ctx.stroke();
}

/* Canvas class */
function Canvas(e) {
    this.o = document.getElementById(e);
    this.ctx = this.o.getContext("2d");
    this.set_size(640, 480);
    this.draw_callback = undefined;
    this.draw_callback_scope = undefined;
}

Canvas.prototype.clear = function () {
    this.ctx.clearRect(0, 0, this.width, this.height);
}

Canvas.prototype.draw = function () {
    this.clear();

    if (this.draw_callback)
        this.draw_callback.call(this.draw_callback_scope, this.ctx);

    //player.move();
    //player.draw(this.ctx);
}

Canvas.prototype.start_draw = function () {
    setInterval(goog.bind(this.draw, this), 10);
}

Canvas.prototype.set_size = function (w, h) {
    this.o.width = w;
    this.o.height = h;
    this.o.style.width = w;
    this.o.style.height = h;
    this.width = w;
    this.height = h;
}

Canvas.prototype.set_draw_callback = function (func, scope) {
    this.draw_callback = func;
    this.draw_callback_scope = scope;
}

/* Game class */
function Game(canvas) {
    this.other_players = new goog.structs.Map;
    this.player = undefined;

    this.canvas = canvas;
    this.canvas.set_draw_callback(this.draw, this);

    this.send_func = undefined;
    
    this.input = new Input();
    this.input.listenKeys(this.on_keys, this);
    this.input.listenMouse(this.on_mouse, this);

    this.tickdelay = 0;

    // have we got user info?
    this.b_user_fetched = false;
    // have we got server settings?
    this.b_info_fetched = false;
    // have we got user's public character info?
    this.b_player_pub_fetched = false;
    // have we got user's private character info?
    this.b_player_priv_fetched = false;

    Game.parsing_table = {};
    Game.parsing_table[proto.runeio.proto.message.SharedCase.SHARED_NOT_SET]
        = { get: undefined, parse: this.parse_empty };
    Game.parsing_table[proto.runeio.proto.message.SharedCase.INFO]
        = { get: proto.runeio.proto.message.prototype.getInfo, parse: this.parse_info };
    Game.parsing_table[proto.runeio.proto.message.SharedCase.ERROR]
        = { get: proto.runeio.proto.message.prototype.getError, parse: this.parse_error };
    Game.parsing_table[proto.runeio.proto.message.SharedCase.USER]
        = { get: proto.runeio.proto.message.prototype.getUser, parse: this.parse_user };
    Game.parsing_table[proto.runeio.proto.message.SharedCase.GAME_STATE]
        = { get: proto.runeio.proto.message.prototype.getGameState, parse: this.parse_game_state };
}

Game.prototype.draw = function (ctx) {
    ctx.fillText("ID: " + this.player.id, 2, 10);
    ctx.fillText("X: " + this.player.x, 2, 20);
    ctx.fillText("Y: " + this.player.y, 2, 30);
    ctx.fillText("VX: " + this.player.vx, 2, 40);
    ctx.fillText("VY: " + this.player.vy, 2, 50);
    ctx.fillText("last_input_id: " + this.player.last_input_id, 2, 60);
    ctx.fillText("last_server_parsed_input_id: " + this.player.last_server_parsed_input_id, 2, 70);

    this.player.draw(ctx);

    this.other_players.forEach(function (player) {
        player.draw(ctx);
    });
}

Game.prototype.set_send_func = function (f) {
    this.send_func = f;
}

Game.prototype.check_game_is_ready = function () {
    if (!this.b_user_fetched
        || !this.b_info_fetched
        || !this.b_player_pub_fetched
        || !this.b_player_priv_fetched)
        return;

    console.log("Starting the game!");
    this.canvas.start_draw();
    this.input.init();
}

Game.prototype.on_keys = function (input) {
    var net_user_input = new proto.runeio.proto.character.user_input();

    net_user_input.setInputId(++this.player.last_input_id);
    net_user_input.setInput(input.serializeInput());

    var net_message = new proto.runeio.proto.message();
    net_message.setUserInput(net_user_input);
    this.send_func(net_message.serializeBinary());
}

Game.prototype.on_mouse = function (input) {
    var net_user_input = new proto.runeio.proto.character.user_input();

    net_user_input.setInputId(++this.player.last_input_id);
    net_user_input.setTargetViewAngle(input.count_angle(this.player.x, this.player.y));

    var net_message = new proto.runeio.proto.message();
    net_message.setUserInput(net_user_input);
    this.send_func(net_message.serializeBinary());
}


Game.prototype.on_player_add = function (player) {
    console.log("New player id: " + player.id);
}

Game.prototype.on_player_delete = function (player) {
    console.log("Deleting player id: " + player.id);
}

Game.prototype.parse_game_state = function (net_chars) {
    if (!net_chars)
        return;

    // skipping gamedata if we still have no our user
    if (!this.b_user_fetched)
        return;

    var net_private = net_chars.getPrivateChar();
    if (net_private) {
        this.player.parse_net_priv(net_private);

        if (!this.b_player_priv_fetched) {
            this.b_player_priv_fetched = true;
            this.check_game_is_ready();
        }
    }

    var net_map = net_chars.getPubCharsMap();
    if (!net_map)
        return;

    // marking all players is_updated = false
    this.other_players.forEach(function (player) {
        player.is_updated = false;
    });

    // updating players with network data
    var it_keys = net_map.keys();
    for (var key = it_keys.next() ; !key.done; key = it_keys.next()) {
        if (this.player.id == key.value) {
            this.player.parse_net_pub(net_map.get(key.value));

            if (!this.b_player_pub_fetched) {
                this.b_player_pub_fetched = true;
                this.check_game_is_ready();
            }
        } else {
            if (this.other_players.containsKey(key.value)) {
                this.other_players.get(key.value).parse_net_pub(net_map.get(key.value));
            } else {
                var player = Player.parse_net_pub(key.value, net_map.get(key.value));
                this.other_players.set(key.value, player);
                this.on_player_add(player);
            }
        }
    }

    // extracting players' id to deletion
    var keys_to_delete = [];
    this.other_players.forEach(function (player) {
        if (!player.is_updated)
            keys_to_delete.push(player.id);
    });

    // deleting players
    for (var i = 0; i < keys_to_delete.length; i++) {
        this.on_player_delete(this.other_players.get(keys_to_delete[i]));
        this.other_players.remove(keys_to_delete[i]);
    }
}

Game.prototype.parse_user = function (net_user) {
    if (!net_user)
        return;

    console.log("NetUser {id: " + net_user.getId() + "}");

    if (!this.b_user_fetched) {
        this.player = new Player(net_user.getId());

        this.b_user_fetched = true;
        this.check_game_is_ready();
    } else {
        if (this.player.id != net_user.getId())
            return;
    }
}

Game.prototype.parse_info = function (net_info) {
    if (!net_info)
        return;

    console.log("NetInfo {tickdelay: " + net_info.getTickdelay() + "}");

    this.tickdelay = net_info.getTickdelay();

    if (!this.b_info_fetched) {
        this.b_info_fetched = true;
        this.check_game_is_ready();
    }
}

Game.prototype.parse_error = function (str) {
    console.log("Server returned error message: " + str);
}

Game.prototype.parse_empty = function () {
    console.log("Server returned empty message");
}

Game.prototype.parse_message = function (bytes) {
    var net_message = proto.runeio.proto.message.deserializeBinary(bytes);
    var parser = Game.parsing_table[net_message.getSharedCase()];
    parser.parse.call(this, parser.get ? parser.get.call(net_message) : undefined);
}

/* WebSocketControl class */
function WebSocketControl(url, reconnect) {
    this.url = url;

    this.ws = new goog.net.WebSocket(reconnect);
    this.ws.listen(goog.net.WebSocket.EventType.OPENED, this.on_open, false, this);
    this.ws.listen(goog.net.WebSocket.EventType.CLOSED, this.on_close, false, this);
    this.ws.listen(goog.net.WebSocket.EventType.ERROR, this.on_error, false, this);
    this.ws.listen(goog.net.WebSocket.EventType.MESSAGE, this.on_message, false, this);

    this.message_listener = undefined;
    this.message_listener_scope = undefined;
}

WebSocketControl.prototype.send = function (data) {
    if (this.ws.isOpen())
        this.ws.send(data);
}

WebSocketControl.prototype.close = function () {
    if (this.ws.isOpen())
        this.ws.close();
}

WebSocketControl.prototype.open = function () {
    this.close();    
    this.ws.open(this.url);
}

WebSocketControl.prototype.listen = function (listener, scope) {
    this.message_listener = listener || undefined;
    this.message_listener_scope = scope || undefined;
}

WebSocketControl.prototype.on_open = function (e) {
    console.log("WebSocket opened to url " + this.url);
}

WebSocketControl.prototype.on_close = function (e) {
    console.log("WebSocket closed to url " + this.url);
}

WebSocketControl.prototype.on_error = function (e) {
    console.log("WebSocket error url: " + this.url + " data: " + e.data);
}

WebSocketControl.prototype.on_message = function (e) {
    var reader = new FileReader();
    reader.addEventListener("loadend", goog.bind(this.invoke_listener, this, reader));
    reader.readAsArrayBuffer(e.message);
}

WebSocketControl.prototype.invoke_listener = function (reader) {
    if (this.message_listener)
        this.message_listener.call(this.message_listener_scope, reader.result);
}


/* Input class */
function Input() {
    this.keys = {
        up: false,
        down: false,
        right: false,
        left: false
    };

    this.mouse = {
        x: 0,
        y: 0
    };

    this.key_change_listener = undefined;
    this.key_change_listener_scope = undefined;

    this.mouse_move_listener = undefined;
    this.mouse_move_listener_scope = undefined;
}

Input.prototype.init = function () {
    goog.events.listen(window, goog.events.EventType.MOUSEMOVE, this.onMouseMove, false, this);
    goog.events.listen(window, goog.events.EventType.KEYUP, this.onKeyUp, false, this);
    goog.events.listen(window, goog.events.EventType.KEYDOWN, this.onKeyDown, false, this);
}

Input.prototype.serializeInput = function () {
    var r = 0;
    r |= !this.keys.up ? 0 : proto.runeio.proto.character.user_input_codes_base.KEY_UP;
    r |= !this.keys.down ? 0 : proto.runeio.proto.character.user_input_codes_base.KEY_DOWN;
    r |= !this.keys.right ? 0 : proto.runeio.proto.character.user_input_codes_base.KEY_RIGHT;
    r |= !this.keys.left ? 0 : proto.runeio.proto.character.user_input_codes_base.KEY_LEFT;
    return r;
}

Input.prototype.listenKeys = function (listener, scope) {
    this.key_change_listener = listener || undefined;
    this.key_change_listener_scope = scope || undefined;
}

Input.prototype.listenMouse = function (listener, scope) {
    this.mouse_move_listener = listener || undefined;
    this.mouse_move_listener_scope = scope || undefined;
}

Input.prototype.onKeyChange = function () {
    if (this.key_change_listener)
        this.key_change_listener.call(this.key_change_listener_scope, this);
}

Input.prototype.onMouseMove = function (e) {
    this.mouse.x = e.clientX;
    this.mouse.y = e.clientY;

    if (this.mouse_move_listener)
        this.mouse_move_listener.call(this.mouse_move_listener_scope, this);
}

Input.prototype.onKeyUp = function (e) {
    var changed = false;
    switch (e.keyCode) {
        case 65: changed = true; this.keys.left = false; break;
        case 87: changed = true; this.keys.up = false; break;
        case 68: changed = true; this.keys.right = false; break;
        case 83: changed = true; this.keys.down = false; break;
    }

    if (changed)
        this.onKeyChange();
}

Input.prototype.onKeyDown = function (e) {
    var changed = false;
    switch (e.keyCode) {
        case 65: changed = true; this.keys.left = true; break;
        case 87: changed = true; this.keys.up = true; break;
        case 68: changed = true; this.keys.right = true; break;
        case 83: changed = true; this.keys.down = true; break;
    }

    if (changed)
        this.onKeyChange();
}

Input.prototype.count_angle = function (x, y) {
    var a_dx = this.mouse.x - x;
    var a_dy = this.mouse.y - y;
    var angle;

    if (a_dx === 0 && a_dy === 0)
        angle = 0;
    else
        angle = Math.acos(a_dx / Math.sqrt(Math.pow(a_dx, 2) + Math.pow(a_dy, 2)));

    if (a_dy < 0)
        angle = 2*Math.PI-angle;

    return angle;
}

/*var player = {
    velocity: 1,
    x: 100,
    y: 100,
    r: 20,
    angle: 0,

    move: function () {
        var dx = 0;
        var dy = 0;
        var pos_message = new proto.character.position();

        // считаем базовый вектор движения
        if (controls.keys.left) dx -= 1;
        if (controls.keys.right) dx += 1;
        if (controls.keys.up) dy -= 1;
        if (controls.keys.down) dy += 1;

        // если ничего не изменилось, не мучаемся
        if (dx === 0 && dy === 0)
            return;

        // нормализуем его, чтобы up+left (к примеру), не был быстрее, чем просто движение вперёд
        // и тут же домножаем на скорость
        var vec_norm = Math.sqrt(Math.pow(dx, 2) + Math.pow(dy, 2))
        dx = dx / vec_norm * this.velocity;
        dy = dy / vec_norm * this.velocity;

        // двигаем персонажа
        this.x += dx;
        this.y += dy;

        // проверяем, не вышел ли он за пределы поля
        if (this.x < 0) this.x = 0;
        if (this.x > canvas.width) this.x = canvas.width;
        if (this.y < 0) this.y = 0;
        if (this.y > canvas.height) this.y = canvas.height;

        // пересчитываем угол взгляда (мы же передвинулись)
        this.count_angle();

        pos_message.setX(this.x);
        pos_message.setY(this.y);
        pos_message.setAngle(this.angle);

        if (ws_control.ws)
            setTimeout(function (data) { ws_control.ws.send(data) }, 0, pos_message.serializeBinary());
    },
};*/