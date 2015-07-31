#include "webdriverhub.h"

WebDriverHub::WebDriverHub(QString host, QString port, DesiredCapabilities* dc) :
    SeleniumServerHub(host, port)
{
    m_dc = dc;
}

WebDriverHub::~WebDriverHub()
{
    qDebug()<<"I am destructor WebDriverHub!";
}

void WebDriverHub::updateCapability(QJsonObject json)
{
    m_dc->setAcceptSslCerts(json["acceptSslCerts"].toBool());
    m_dc->setApplicationCacheEnabled(json["applicationCacheEnabled"].toBool());
    m_dc->setDatabaseEnabled(json["databaseEnabled"].toBool());
    m_dc->setJavascriptEnabled(json["javascriptEnabled"].toBool());
    m_dc->setLocationContextEnabled(json["locationContextEnabled"].toBool());
    m_dc->setNativeEvents(json["nativeEvents"].toBool());
    m_dc->setRotatable(json["rotatable"].toBool());
    m_dc->setWebStorageEnabled(json["webStorageEnabled"].toBool());
    m_dc->setPlatform(Platform::fromString(json["platform"].toString()));
    m_dc->setVersion(json["version"].toString());
}

DesiredCapabilities* WebDriverHub::capabilities(bool refresh)
{
    if(!refresh) {
        return m_dc;
    }

    QJsonObject result = post("");

    if(result["value"].isObject()) {

        updateCapability(result["value"].toObject());
        return m_dc;
    }

    qDebug()<<"Error refresh capabilities!";

    return 0;
}

/**
 * GET /status
 *
 * Query the server's current status.
 * The server should respond with a general "HTTP 200 OK" response if it is alive and accepting commands.
 * The response body should be a JSON object describing the state of the server.
 * All server implementations should return two basic objects describing the server's current platform and
 * when the server was built. All fields are optional; if omitted, the client should assume the value is uknown.
 * Furthermore, server implementations may include additional fields not listed here.
 *
 * |-----------------------------------------------------------------------------------------------------------|
 * | Key            | Type   | Description                                                                     |
 * |-------------------------|---------------------------------------------------------------------------------|
 * | build          | object |                                                                                 |
 * |-----------------------------------------------------------------------------------------------------------|
 * | build.version  | string | A generic release label (i.e. "2.0rc3")                                         |
 * |-----------------------------------------------------------------------------------------------------------|
 * | build.revision | string | The revision of the local source control client from which the server was built |
 * |-----------------------------------------------------------------------------------------------------------|
 * | build.time     | string | A timestamp from when the server was built.                                     |
 * |-----------------------------------------------------------------------------------------------------------|
 * | os             | object |                                                                                 |
 * |-----------------------------------------------------------------------------------------------------------|
 * | os.arch        | string | The current system architecture.                                                |
 * |-----------------------------------------------------------------------------------------------------------|
 * | os.name        | string | The name of the operating system the server is currently running on:            |
 * |                |        | "windows", "linux", etc.                                                        |
 * |-----------------------------------------------------------------------------------------------------------|
 * | os.version     | string | The operating system version.                                                   |
 * |-----------------------------------------------------------------------------------------------------------|
 *
 * Returns:
 *      {object} - An object describing the general status of the server.
 */
QJsonObject WebDriverHub::status()
{
    return getValueObject("status");
}

/**
 * POST /session
 *
 * Create a new session.
 * The server should attempt to create a session that most closely matches the desired and required capabilities.
 * Required capabilities have higher priority than desired capabilities and must be set for the session to be created.
 *
 * JSON Parameters:
 *      desiredCapabilities  - {object} An object describing the session's desired capabilities.
 *      requiredCapabilities - {object} An object describing the session's required capabilities (Optional).
 *
 * Returns:
 *      {object} - An object describing the session's capabilities.
 *
 * Potential Errors:
 *      SessionNotCreatedException - If a required capability could not be set.
 */
void WebDriverHub::startSession()
{
    QJsonObject desiredCapabilities;
    desiredCapabilities["browserName"] = QString(BrowserType::toString(m_dc->browser()));

    if(NULL != m_dc->proxy()) {

        QJsonObject proxy;
        proxy["proxyType"] = QString(Proxy::toString(m_dc->proxy()->proxyType()));

        if(m_dc->proxy()->proxyType() == Proxy::PAC) {
            proxy["proxyAutoconfigUrl"] = QString(m_dc->proxy()->autoconfigUrl());
        }

        if(m_dc->proxy()->proxyType() != Proxy::MANUAL) {

            if(!m_dc->proxy()->socksUsername().isEmpty()) {
                proxy["socksUsername"] = m_dc->proxy()->socksUsername();
            }
            if(!m_dc->proxy()->socksPassword().isEmpty()) {
                proxy["socksPassword"] = m_dc->proxy()->socksPassword();
            }
        }

        desiredCapabilities["proxy"] = proxy;
    }

    if(m_dc->hasJavascriptEnabled()) {
        desiredCapabilities["javascriptEnabled"] = m_dc->isJavascriptEnabled();
    }

    if(m_dc->hasDatabaseEnabled()) {
        desiredCapabilities["databaseEnabled"] = m_dc->isDatabaseEnabled();
    }

    if(m_dc->hasLocationContextEnabled()) {
        desiredCapabilities["locationContextEnabled"] = m_dc->isLocationContextEnabled();
    }

    if(m_dc->hasApplicationCacheEnabled()) {
        desiredCapabilities["applicationCacheEnabled"] = m_dc->isApplicationCacheEnabled();
    }

    if(m_dc->hasBrowserConnectionEnabled()) {
        desiredCapabilities["browserConnectionEnabled"] = m_dc->isBrowserConnectionEnabled();
    }

    if(m_dc->hasWebStorageEnabled()) {
        desiredCapabilities["webStorageEnabled"] = m_dc->isWebStorageEnabled();
    }

    if(m_dc->hasAcceptSslCerts()) {
        desiredCapabilities["acceptSslCerts"] = m_dc->isAcceptSslCerts();
    }

    if(m_dc->hasRotatable()) {
        desiredCapabilities["rotatable"] = m_dc->isRotatable();
    }

    if(m_dc->hasNativeEvents()) {
        desiredCapabilities["nativeEvents"] = m_dc->isNativeEvents();
    }

    if(m_dc->hasUnexpectedAlertBehaviour()) {
        desiredCapabilities["unexpectedAlertBehaviour"] = UnexpectedAlertBehaviour::toString(m_dc->unexpectedAlertBehaviour());
    }

    if(m_dc->hasElementScrollBehavior()) {
        desiredCapabilities["elementScrollBehavior"] = m_dc->elementScrollBehavior();
    }

    QJsonObject rawBody;
    rawBody["desiredCapabilities"] = desiredCapabilities;

    QJsonObject result = post("session", rawBody);//create new session

    if(result["sessionId"].isString()) {
        m_sessionId = result["sessionId"].toString();
    }

    if(result["value"].isObject()) {
        updateCapability(result["value"].toObject());
    }

   m_urlHub.append("session/").append(m_sessionId).append("/");
}

/**
 * DELETE /session/:sessionId
 *
 * Delete the session.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 */
void WebDriverHub::deleteSession()
{
    deleteMethod("");
}

/**
 * POST /session/:sessionId/timeouts
 *
 * Configure the amount of time that a particular type of operation can execute for before they are aborted and
 * a |Timeout| error is returned to the client.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      type - {string} The type of operation to set the timeout for.
 *             Valid values are: "script" for script timeouts, "implicit" for modifying
 *             the implicit wait timeout and "page load" for setting a page load timeout.
 *      ms   - {number} The amount of time, in milliseconds, that time-limited commands are permitted to run.
 */
void WebDriverHub::setTimeouts(QString type, int ms)
{
    QJsonObject json;
    json["type"] = type;
    json["ms"]   = ms;

    post("timeouts", json);
}

/**
 * POST /session/:sessionId/timeouts/async_script
 *
 * Set the amount of time, in milliseconds, that asynchronous scripts
 * executed by /session/:sessionId/execute_async are permitted to run before they are aborted and
 * a |Timeout| error is returned to the client.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      ms - {number} The amount of time, in milliseconds, that time-limited commands are permitted to run.
 */
void WebDriverHub::setTimeoutsAsyncScript(int ms)
{
    QJsonObject json;
    json["ms"] = ms;

    post("timeouts/async_script", json);
}

/**
 * POST /session/:sessionId/timeouts/implicit_wait
 *
 * Set the amount of time the driver should wait when searching for elements.
 * When searching for a single element, the driver should poll the page until an element is found or
 * the timeout expires, whichever occurs first. When searching for multiple elements,
 * the driver should poll the page until at least one element is found or the timeout expires,
 * at which point it should return an empty list.
 *
 * If this command is never sent, the driver should default to an implicit wait of 0ms.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      ms - {number} The amount of time to wait, in milliseconds. This value has a lower bound of 0.
 */
void WebDriverHub::setTimeoutsImplicitWait(int ms)
{
    QJsonObject json;
    json["ms"] = ms;

    post("timeouts/implicit_wait", json);
}

/**
 * GET /session/:sessionId/window_handle
 *
 * Retrieve the current window handle.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {string} The current window handle.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 */
QString WebDriverHub::getWindowHandle()
{
    return getValueString("window_handle");
}

/**
 * GET /session/:sessionId/window_handles
 *
 * Retrieve the list of all window handles available to the session.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {Array.<string>} A list of window handles.
 */
QStringList WebDriverHub::getWindowHandles()
{
    return getValueArrayString("window_handles");
}

/**
 * GET /session/:sessionId/url
 *
 * Retrieve the URL of the current page.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {string} The current URL.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 */
QString WebDriverHub::getUrl()
{
    return getValueString("url");
}

/**
 * POST /session/:sessionId/url
 *
 * Navigate to a new URL.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      url - {string} The URL to navigate to.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 */
void WebDriverHub::setUrl(QString url)
{
    QJsonObject json;
    json["url"] = "http://" + url.replace("http://", "");

    post("url", json);
}

/**
 * POST /session/:sessionId/forward
 *
 * Navigate forwards in the browser history, if possible.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 */
void WebDriverHub::forward()
{
    post("forward");
}

/**
 * POST /session/:sessionId/back
 *
 * Navigate backwards in the browser history, if possible.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 */
void WebDriverHub::back()
{
    post("back");
}

/**
 * POST /session/:sessionId/refresh
 *
 * Refresh the current page.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 */
void WebDriverHub::refresh()
{
    post("refresh");
}

/**
 * POST /session/:sessionId/execute
 *
 * Inject a snippet of JavaScript into the page for execution in the context of the currently selected frame.
 * The executed script is assumed to be synchronous and the result of evaluating the script is returned to the client.
 *
 * The script argument defines the script to execute in the form of a function body.
 * The value returned by that function will be returned to the client.
 * The function will be invoked with the provided args array and the values may be accessed via
 * the arguments object in the order specified.
 *
 * Arguments may be any JSON-primitive, array, or JSON object. JSON objects that define
 * a WebElement reference will be converted to the corresponding DOM element.
 * Likewise, any WebElements in the script result will be returned to the client as WebElement JSON objects.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      script - {string} The script to execute.
 *      args   - {Array.<*>} The script arguments.
 *
 * Returns:
 *      {*} The script result.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If one of the script arguments is a WebElement that is not attached to the page's DOM.
 *      JavaScriptError       - If the script throws an Error.
 */
QJsonObject WebDriverHub::executeJS(QString script, QStringList args)
{
    QJsonArray arr;

    if(args.size() > 0) {

        for(int i = 0; i < args.size(); i++) {
            arr.append(QString(args.at(i)));
        }
    }

    QJsonObject json;
    json["script"] = script;
    json["args"] = arr;

    return post("execute", json);
}

/**
 * POST /session/:sessionId/execute_async
 *
 * Inject a snippet of JavaScript into the page for execution in the context of the currently selected frame.
 * The executed script is assumed to be asynchronous and must signal that is done by invoking the provided callback,
 * which is always provided as the final argument to the function.
 * The value to this callback will be returned to the client.
 *
 * Asynchronous script commands may not span page loads. If an unload event is fired while waiting for
 * a script result, an error should be returned to the client.
 *
 * The script argument defines the script to execute in teh form of a function body.
 * The function will be invoked with the provided args array and the values may be accessed via
 * the arguments object in the order specified. The final argument will always be a callback function
 * that must be invoked to signal that the script has finished.
 *
 * Arguments may be any JSON-primitive, array, or JSON object.
 * JSON objects that define a WebElement reference will be converted to the corresponding DOM element.
 * Likewise, any WebElements in the script result will be returned to the client as WebElement JSON objects.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      script - {string} The script to execute.
 *      args   - {Array.<*>} The script arguments.
 *
 * Returns:
 *      {*} The script result.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If one of the script arguments is a WebElement that is not attached to the page's DOM.
 *      Timeout               - If the script callback is not invoked before the timout expires.
 *                              Timeouts are controlled by the /session/:sessionId/timeout/async_script command.
 *      JavaScriptError       - If the script throws an Error or if an unload event is fired while waiting for the
 *                              script to finish.
 */
QJsonObject WebDriverHub::executeAsyncJS(QString script, QStringList args)
{
    QJsonArray arr;

    if(args.size() > 0) {

        for(int i = 0; i < args.size(); i++) {
            arr.append(QString(args.at(i)));
        }
    }

    QJsonObject json;
    json["script"] = script;
    json["args"] = arr;

    return post("execute_async", json);
}

/**
 * GET /session/:sessionId/screenshot
 *
 * Take a screenshot of the current page.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {string} The screenshot as a base64 encoded PNG.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 */
QString WebDriverHub::getScreenshot()
{
    return getValueString("screenshot");
}

/**
 * GET /session/:sessionId/ime/available_engines
 *
 * List all available engines on the machine. To use an engine, it has to be present in this list.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {Array.<string>} A list of available engines
 *
 * Potential Errors:
 *      ImeNotAvailableException - If the host does not support IME
 */
QStringList WebDriverHub::getImeAvailableEngines()
{
    return getValueArrayString("ime/available_engines");
}

/**
 * GET /session/:sessionId/ime/active_engine
 *
 * Get the name of the active IME engine. The name string is platform specific.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {string} The name of the active IME engine.
 *
 * Potential Errors:
 *      ImeNotAvailableException - If the host does not support IME
 */
QString WebDriverHub::getImeActiveEngine()
{
    return getValueString("ime/active_engine");
}

/**
 * GET /session/:sessionId/ime/activated
 *
 * Indicates whether IME input is active at the moment (not if it's available).
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {boolean} true if IME input is available and currently active, false otherwise
 *
 * Potential Errors:
 *      ImeNotAvailableException - If the host does not support IME
 */
bool WebDriverHub::isImeActivated()
{
    return getValueBool("ime/activated");
}

/**
 * POST /session/:sessionId/ime/deactivate
 *
 * De-activates the currently-active IME engine.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Potential Errors:
 *      ImeNotAvailableException - If the host does not support IME
 */
void WebDriverHub::imeDeactivate()
{
    post("ime/deactivate");
}

/**
 * POST /session/:sessionId/ime/activate
 *
 * Make an engines that is available (appears on the list returned by getAvailableEngines) active.
 * After this call, the engine will be added to the list of engines loaded in the IME daemon and
 * the input sent using sendKeys will be converted by the active engine.
 * Note that this is a platform-independent method of activating IME (the platform-specific way
 * being using keyboard shortcuts
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      engine - {string} Name of the engine to activate.
 *
 * Potential Errors:
 *      ImeActivationFailedException - If the engine is not available or if the activation fails for other reasons.
 *      ImeNotAvailableException     - If the host does not support IME
 */
void WebDriverHub::imeActivate(QString engine)
{
    QJsonObject json;
    json["engine"] = engine;

    post("ime/activate", json);
}

/**
 * POST /session/:sessionId/frame
 *
 * Change focus to another frame on the page. If the frame id is null,
 * the server should switch to the page's default content.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      id - {string|number|null|WebElement JSON Object} Identifier for the frame to change focus to.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 *      NoSuchFrame  - If the frame specified by id cannot be found.
 */
void WebDriverHub::changeFrame(QString id)
{
    QJsonObject json;
    json["id"] = id;

    post("frame", json);
}

void WebDriverHub::changeFrame(int id)
{
    QJsonObject json;
    json["id"] = id;

    post("frame", json);
}

void WebDriverHub::changeFrame(QJsonObject id)
{
    QJsonObject json;
    json["id"] = id;

    post("frame", json);
}

void WebDriverHub::changeFrame()
{
    QJsonObject json;
    post("frame", json);
}

/**
 * POST /session/:sessionId/frame/parent
 *
 * Change focus to the parent context. If the current context is the top level browsing context,
 * the context remains unchanged.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 */
void WebDriverHub::changeFrameParent()
{
    post("frame/parent");
}

/**
 * POST /session/:sessionId/window
 *
 * Change focus to another window. The window to change focus to may be specified by its
 * server assigned window handle, or by the value of its name attribute.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      name - {string} The window to change focus to.
 *
 * Potential Errors:
 *      NoSuchWindow - If the window specified by name cannot be found.
 */
void WebDriverHub::changeWindow(QString name)
{
    QJsonObject json;
    json["name"] = name;

    post("window", json);
}

/**
 * DELETE /session/:sessionId/window
 *
 * Close the current window.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window is already closed
 */
void WebDriverHub::deleteWindow()
{
    deleteMethod("window");
}

/**
 * POST /session/:sessionId/window/:windowHandle/size
 *
 * Change the size of the specified window.
 * If the :windowHandle URL parameter is "current", the currently active window will be resized.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      width  - {number} The new window width.
 *      height - {number} The new window height.
 */
void WebDriverHub::setWindowSize(QString handle, int width, int height)
{
    QJsonObject json;
    json["width"] = width;
    json["height"] = height;

    post("window/" + handle + "/size", json);
}

/**
 * GET /session/:sessionId/window/:windowHandle/size
 *
 * Get the size of the specified window. If the :windowHandle URL parameter is "current",
 * the size of the currently active window will be returned.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {width: number, height: number} The size of the window.
 *
 * Potential Errors:
 *      NoSuchWindow - If the specified window cannot be found.
 */
QSize WebDriverHub::getWindowSize(QString handle)
{
    QSize size;
    size.setWidth(0);
    size.setHeight(0);

    QJsonObject result = getValueObject("window/" + handle + "/size");

    if(result["width"].isDouble()) {
        size.setWidth(result["width"].toInt());
    } else {
        qDebug()<<"Error get width value";
    }

    if(result["height"].isDouble()) {
        size.setHeight(result["height"].toInt());
    } else {
        qDebug()<<"Error get height value";
    }

    return size;
}

/**
 * POST /session/:sessionId/window/:windowHandle/position
 *
 * Change the position of the specified window. If the :windowHandle URL parameter is "current",
 * the currently active window will be moved.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      x - {number} The X coordinate to position the window at, relative to the upper left corner of the screen.
 *      y - {number} The Y coordinate to position the window at, relative to the upper left corner of the screen.
 *
 * Potential Errors:
 *      NoSuchWindow - If the specified window cannot be found.
 */
void WebDriverHub::setWindowPosition(QString handle, int x, int y)
{
    QJsonObject json;
    json["x"] = x;
    json["y"] = y;

    post("window/" + handle + "/position", json);
}

/**
 * GET /session/:sessionId/window/:windowHandle/position
 *
 * Get the position of the specified window. If the :windowHandle URL parameter is "current",
 * the position of the currently active window will be returned.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {x: number, y: number} The X and Y coordinates for the window, relative to the upper left corner of the screen.
 *
 * Potential Errors:
 *      NoSuchWindow - If the specified window cannot be found.
 */
QPoint WebDriverHub::getWindowPosition(QString handle)
{
    QPoint pos;
    pos.setX(0);
    pos.setY(0);

    QJsonObject result = getValueObject("window/" + handle + "/position");

    if(result["x"].isDouble()) {
        pos.setX(result["x"].toInt());
    } else {
        qDebug()<<"Error get x value";
    }

    if(result["y"].isDouble()) {
        pos.setY(result["y"].toInt());
    } else {
        qDebug()<<"Error get y value";
    }

    return pos;
}

/**
 * POST /session/:sessionId/window/:windowHandle/maximize
 *
 * Maximize the specified window if not already maximized. If the :windowHandle URL parameter is "current",
 * the currently active window will be maximized.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Potential Errors:
 *      NoSuchWindow - If the specified window cannot be found.
 */
void WebDriverHub::windowMaximize(QString handle)
{
    post("window/" + handle + "/maximize");
}

/**
 * GET /session/:sessionId/cookie
 *
 * Retrieve all cookies visible to the current page.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {Array.<object>} A list of cookies.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 */
QList<QJsonObject> WebDriverHub::getAllCookie()
{
    QList<QJsonObject> cookie;
    QJsonArray result = getValueArray("cookie");

    for(int i = 0; i < result.size(); i++) {

        if(result.at(i).isObject()) {
            cookie.push_back(result.at(i).toObject());
        } else {
            qDebug()<<"Error get jsonobject value";
        }
    }

    return cookie;
}

/**
 * POST /session/:sessionId/cookie
 *
 * Set a cookie. If the cookie path is not specified, it should be set to "/".
 * Likewise, if the domain is omitted, it should default to the current page's domain.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      cookie - {object} A JSON object defining the cookie to add.
 */
void WebDriverHub::setCookie(QJsonObject cookie)
{
    QJsonObject json;
    json["cookie"] = cookie;

    post("cookie", json);
}

/**
 * DELETE /session/:sessionId/cookie
 *
 * Delete all cookies visible to the current page.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Potential Errors:
 *      InvalidCookieDomain - If the cookie's domain is not visible from the current page.
 *      NoSuchWindow        - If the currently selected window has been closed.
 *      UnableToSetCookie   - If attempting to set a cookie on a page that does
 *                            not support cookies (e.g. pages with mime-type text/plain).
 */
void WebDriverHub::deleteAllCookie()
{
    deleteMethod("cookie");
}

/**
 * DELETE /session/:sessionId/cookie/:name
 *
 * Delete the cookie with the given name. This command should be a no-op if there
 * is no such cookie visible to the current page.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :name - The name of the cookie to delete.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 */
void WebDriverHub::deleteCookie(QString name)
{
    deleteMethod("cookie/" + name);
}

/**
 * GET /session/:sessionId/source
 *
 * Get the current page source.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {string} The current page source.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 */
QString WebDriverHub::getSource()
{
    return getValueString("source");
}


/**
 * GET /session/:sessionId/title
 *
 * Get the current page title.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {string} The current page title.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 */
QString WebDriverHub::getTitle()
{
    return getValueString("title");
}

/**
 * POST /session/:sessionId/element
 *
 * Search for an element on the page, starting from the document root.
 * The located element will be returned as a WebElement JSON object.
 * The table below lists the locator strategies that each server should support.
 * Each locator must return the first matching element located in the DOM.
 *
 * |------------------------------------------------------------------------------------------------------|
 * | Strategy          | Description                                                                      |
 * |------------------------------------------------------------------------------------------------------|
 * | class name        | Returns an element whose class name contains the search value;                   |
 * |                   | compound class names are not permitted.                                          |
 * |------------------------------------------------------------------------------------------------------|
 * | css selector      | Returns an element matching a CSS selector.                                      |
 * |------------------------------------------------------------------------------------------------------|
 * | id                | Returns an element whose ID attribute matches the search value.                  |
 * |------------------------------------------------------------------------------------------------------|
 * | name              | Returns an element whose NAME attribute matches the search value.                |
 * |------------------------------------------------------------------------------------------------------|
 * | link text         | Returns an anchor element whose visible text matches the search value.           |
 * |------------------------------------------------------------------------------------------------------|
 * | partial link text | Returns an anchor element whose visible text partially matches the search value. |
 * |------------------------------------------------------------------------------------------------------|
 * | tag name	       | Returns an element whose tag name matches the search value.                      |
 * |------------------------------------------------------------------------------------------------------|
 * | xpath             | Returns an element matching an XPath expression.                                 |
 * |------------------------------------------------------------------------------------------------------|
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      using - {string} The locator strategy to use.
 *      value - {string} The The search target.
 *
 * Returns:
 *      {ELEMENT:string} A WebElement JSON object for the located element.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 *      NoSuchElement - If the element cannot be found.
 *      XPathLookupError - If using XPath and the input expression is invalid.
 */
QString WebDriverHub::getElement(QString by, QString value)
{
    QJsonObject json;
    json["using"] = by;
    json["value"] = value;

    QJsonObject result = post("element", json);

    QString id;

    if(result["value"].isObject()) {
        QJsonObject element = result["value"].toObject();

        if(element["ELEMENT"].isString()) {
            id = element["ELEMENT"].toString();
        } else {
            qDebug()<<"Error get ELEMENT";
        }

    } else {
        qDebug()<<"Error get object value";
    }

    return id;

}

/**
 * POST /session/:sessionId/elements
 *
 * Search for multiple elements on the page, starting from the document root.
 * The located elements will be returned as a WebElement JSON objects.
 * The table below lists the locator strategies that each server should support.
 * Elements should be returned in the order located in the DOM.
 *
 * |------------------------------------------------------------------------------------------------------|
 * | Strategy          | Description                                                                      |
 * |------------------------------------------------------------------------------------------------------|
 * | class name        | Returns an element whose class name contains the search value;                   |
 * |                   | compound class names are not permitted.                                          |
 * |------------------------------------------------------------------------------------------------------|
 * | css selector      | Returns an element matching a CSS selector.                                      |
 * |------------------------------------------------------------------------------------------------------|
 * | id                | Returns an element whose ID attribute matches the search value.                  |
 * |------------------------------------------------------------------------------------------------------|
 * | name              | Returns an element whose NAME attribute matches the search value.                |
 * |------------------------------------------------------------------------------------------------------|
 * | link text         | Returns an anchor element whose visible text matches the search value.           |
 * |------------------------------------------------------------------------------------------------------|
 * | partial link text | Returns an anchor element whose visible text partially matches the search value. |
 * |------------------------------------------------------------------------------------------------------|
 * | tag name	       | Returns an element whose tag name matches the search value.                      |
 * |------------------------------------------------------------------------------------------------------|
 * | xpath             | Returns an element matching an XPath expression.                                 |
 * |------------------------------------------------------------------------------------------------------|
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      using - {string} The locator strategy to use.
 *      value - {string} The The search target.
 *
 * Returns:
 *      {Array.<{ELEMENT:string}>} A list of WebElement JSON objects for the located elements.
 *
 * Potential Errors:
 *      NoSuchWindow     - If the currently selected window has been closed.
 *      XPathLookupError - If using XPath and the input expression is invalid.
 */
QStringList WebDriverHub::getElements(QString by, QString value)
{
    QJsonObject json;
    json["using"] = by;
    json["value"] = value;

    QJsonObject result = post("elements", json);

    QStringList ids;

    if(result["value"].isArray()) {

        QJsonArray array = result["value"].toArray();

        for(int i = 0; i < array.size(); i++) {

            if(array.at(i).isObject()) {

                QJsonObject element = array.at(i).toObject();

                if(element["ELEMENT"].isString()) {
                    ids << element["ELEMENT"].toString();
                } else {
                    qDebug()<<"Error get ELEMENT";
                }

            } else {
                qDebug()<<"Error get object value";
            }
        }

    } else {
        qDebug()<<"Error get array value";
    }

    return ids;
}

/**
 * POST /session/:sessionId/element/active
 *
 * Get the element on the page that currently has focus. The element will be returned as a WebElement JSON object.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {ELEMENT:string} A WebElement JSON object for the active element.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 */
QString WebDriverHub::getElementActive()
{
    QJsonObject result = post("element/active");

    QString id;

    if(result["value"].isObject()) {
        QJsonObject elm = result["value"].toObject();

        if(elm["ELEMENT"].isString()) {
            id = elm["ELEMENT"].toString();
        } else {
            qDebug()<<"Error get ELEMENT";
        }
    } else {
        qDebug()<<"Error get object value";
    }

    return id;
}

/**
 * GET /session/:sessionId/element/:id
 *
 * Describe the identified element.
 *
 * Note: This command is reserved for future use; its return type is currently undefined.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 */
void WebDriverHub::elementId(QString id)
{
    get("element/" + id);
}

/**
 * POST /session/:sessionId/element/:id/element
 *
 * Search for an element on the page, starting from the identified element.
 * The located element will be returned as a WebElement JSON object.
 * The table below lists the locator strategies that each server should support.
 * Each locator must return the first matching element located in the DOM.
 *
 * |------------------------------------------------------------------------------------------------------|
 * | Strategy          | Description                                                                      |
 * |------------------------------------------------------------------------------------------------------|
 * | class name        | Returns an element whose class name contains the search value;                   |
 * |                   | compound class names are not permitted.                                          |
 * |------------------------------------------------------------------------------------------------------|
 * | css selector      | Returns an element matching a CSS selector.                                      |
 * |------------------------------------------------------------------------------------------------------|
 * | id                | Returns an element whose ID attribute matches the search value.                  |
 * |------------------------------------------------------------------------------------------------------|
 * | name              | Returns an element whose NAME attribute matches the search value.                |
 * |------------------------------------------------------------------------------------------------------|
 * | link text         | Returns an anchor element whose visible text matches the search value.           |
 * |------------------------------------------------------------------------------------------------------|
 * | partial link text | Returns an anchor element whose visible text partially matches the search value. |
 * |------------------------------------------------------------------------------------------------------|
 * | tag name	       | Returns an element whose tag name matches the search value.                      |
 * |------------------------------------------------------------------------------------------------------|
 * | xpath             | Returns an element matching an XPath expression.                                 |
 * |------------------------------------------------------------------------------------------------------|
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * JSON Parameters:
 *      using - {string} The locator strategy to use.
 *      value - {string} The The search target.
 *
 * Returns:
 *      {ELEMENT:string} A WebElement JSON object for the located element.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 *      NoSuchElement         - If the element cannot be found.
 *      XPathLookupError      - If using XPath and the input expression is invalid.
 */
QString WebDriverHub::getElementByElement(QString id, QString by, QString value)
{
    QJsonObject json;
    json["using"] = by;
    json["value"] = value;

    QJsonObject result = post("element/" + id + "/element", json);

    QString eId;

    if(result["value"].isObject()) {
        QJsonObject element = result["value"].toObject();
        eId = element["ELEMENT"].isString();
    } else {
        qDebug()<<"Error get object value";
    }

    return eId;
}

/**
 * POST /session/:sessionId/element/:id/elements
 *
 * Search for multiple elements on the page, starting from the identified element.
 * The located elements will be returned as a WebElement JSON objects.
 * The table below lists the locator strategies that each server should support.
 * Elements should be returned in the order located in the DOM.
 *
 * |------------------------------------------------------------------------------------------------------|
 * | Strategy          | Description                                                                      |
 * |------------------------------------------------------------------------------------------------------|
 * | class name        | Returns an element whose class name contains the search value;                   |
 * |                   | compound class names are not permitted.                                          |
 * |------------------------------------------------------------------------------------------------------|
 * | css selector      | Returns an element matching a CSS selector.                                      |
 * |------------------------------------------------------------------------------------------------------|
 * | id                | Returns an element whose ID attribute matches the search value.                  |
 * |------------------------------------------------------------------------------------------------------|
 * | name              | Returns an element whose NAME attribute matches the search value.                |
 * |------------------------------------------------------------------------------------------------------|
 * | link text         | Returns an anchor element whose visible text matches the search value.           |
 * |------------------------------------------------------------------------------------------------------|
 * | partial link text | Returns an anchor element whose visible text partially matches the search value. |
 * |------------------------------------------------------------------------------------------------------|
 * | tag name	       | Returns an element whose tag name matches the search value.                      |
 * |------------------------------------------------------------------------------------------------------|
 * | xpath             | Returns an element matching an XPath expression.                                 |
 * |------------------------------------------------------------------------------------------------------|
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * JSON Parameters:
 *      using - {string} The locator strategy to use.
 *      value - {string} The The search target.
 *
 * Returns:
 *      {Array.<{ELEMENT:string}>} A list of WebElement JSON objects for the located elements.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 *      XPathLookupError      - If using XPath and the input expression is invalid.
 */
QStringList WebDriverHub::getElementsByElement(QString id, QString by, QString value)
{
    QJsonObject json;
    json["using"] = by;
    json["value"] = value;

    QJsonObject result = post("element/" + id + "/elements", json);

    QStringList ids;

    if(result["value"].isArray()) {

        QJsonArray array = result["value"].toArray();

        for(int i = 0; i < array.size(); i++) {

            if(array.at(i).isObject()) {

                QJsonObject element = array.at(i).toObject();

                if(element["ELEMENT"].isString()) {
                    ids << element["ELEMENT"].toString();
                } else {
                    qDebug()<<"Error get ELEMENT";
                }

            } else {
                qDebug()<<"Error get object value";
            }
        }

    } else {
        qDebug()<<"Error get array value";
    }

    return ids;
}

/**
 * POST /session/:sessionId/element/:id/click
 *
 * Click on an element.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 *      ElementNotVisible     - If the referenced element is not visible on the page
 *                              (either is hidden by CSS, has 0-width, or has 0-height)
 */
void WebDriverHub::elementClick(QString id)
{
    post("element/" + id + "/click");
}

/**
 * POST /session/:sessionId/element/:id/submit
 *
 * Submit a FORM element. The submit command may also be applied to any element that is a descendant of a FORM element.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 */
void WebDriverHub::elementSubmit(QString id)
{
    post("element/" + id + "/submit");
}

/**
 * GET /session/:sessionId/element/:id/text
 *
 * Returns the visible text for the element.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 */
QString WebDriverHub::getElementText(QString id)
{
    return getValueString("element/" + id + "/text");
}

/**
 * POST /session/:sessionId/element/:id/value
 *
 * Send a sequence of key strokes to an element.
 *
 * Any UTF-8 character may be specified, however, if the server does not support native key events,
 * it should simulate key strokes for a standard US keyboard layout. The Unicode Private Use Area code points,
 * 0xE000-0xF8FF, are used to represent pressable, non-text keys (see table below).
 *
 * |-----------------------|
 * | Key         | Code    |
 * |-----------------------|
 * | NULL         | U+E000 |
 * |-----------------------|
 * | Cancel       | U+E001 |
 * |-----------------------|
 * | Help         | U+E002 |
 * |-----------------------|
 * | Back space   | U+E003 |
 * |-----------------------|
 * | Tab          | U+E004 |
 * |-----------------------|
 * | Clear        | U+E005 |
 * |-----------------------|
 * | Return1      | U+E006 |
 * |-----------------------|
 * | Enter1       | U+E007 |
 * |-----------------------|
 * | Shift        | U+E008 |
 * |-----------------------|
 * | Control      | U+E009 |
 * |-----------------------|
 * | Alt          | U+E00A |
 * |-----------------------|
 * | Pause        | U+E00B |
 * |-----------------------|
 * | Escape       | U+E00C |
 * |-----------------------|
 * | Space        | U+E00D |
 * |-----------------------|
 * | Pageup       | U+E00E |
 * |-----------------------|
 * | Pagedown     | U+E00F |
 * |-----------------------|
 * | End          | U+E010 |
 * |-----------------------|
 * | Home         | U+E011 |
 * |-----------------------|
 * | Left arrow   | U+E012 |
 * |-----------------------|
 * | Up arrow     | U+E013 |
 * |-----------------------|
 * | Right arrow  | U+E014 |
 * |-----------------------|
 * | Down arrow   | U+E015 |
 * |-----------------------|
 * | Insert       | U+E016 |
 * |-----------------------|
 * | Delete       | U+E017 |
 * |-----------------------|
 * | Semicolon    | U+E018 |
 * |-----------------------|
 * | Equals       | U+E019 |
 * |-----------------------|
 * | Numpad 0     | U+E01A |
 * |-----------------------|
 * | Numpad 1     | U+E01B |
 * |-----------------------|
 * | Numpad 2     | U+E01C |
 * |-----------------------|
 * | Numpad 3     | U+E01D |
 * |-----------------------|
 * | Numpad 4     | U+E01E |
 * |-----------------------|
 * | Numpad 5     | U+E01F |
 * |-----------------------|
 * | Numpad 6     | U+E020 |
 * |-----------------------|
 * | Numpad 7     | U+E021 |
 * |-----------------------|
 * | Numpad 8     | U+E022 |
 * |-----------------------|
 * | Numpad 9     | U+E023 |
 * |-----------------------|
 * | Multiply     | U+E024 |
 * |-----------------------|
 * | Add          | U+E025 |
 * |-----------------------|
 * | Separator    | U+E026 |
 * |-----------------------|
 * | Subtract     | U+E027 |
 * |-----------------------|
 * | Decimal      | U+E028 |
 * |-----------------------|
 * | Divide       | U+E029 |
 * |-----------------------|
 * | F1           | U+E031 |
 * |-----------------------|
 * | F2           | U+E032 |
 * |-----------------------|
 * | F3           | U+E033 |
 * |-----------------------|
 * | F4           | U+E034 |
 * |-----------------------|
 * | F5           | U+E035 |
 * |-----------------------|
 * | F6           | U+E036 |
 * |-----------------------|
 * | F7           | U+E037 |
 * |-----------------------|
 * | F8           | U+E038 |
 * |-----------------------|
 * | F9           | U+E039 |
 * |-----------------------|
 * | F10          | U+E03A |
 * |-----------------------|
 * | F11          | U+E03B |
 * |-----------------------|
 * | F12          | U+E03C |
 * |-----------------------|
 * | Command/Meta | U+E03D |
 * |-----------------------|
 *
 * The return key is not the same as the enter key.
 *
 * The server must process the key sequence as follows:
 *  - Each key that appears on the keyboard without requiring modifiers are sent as a keydown followed by a key up.
 *  - If the server does not support native events and must simulate key strokes with JavaScript,
 *    it must generate keydown, keypress, and keyup events, in that order. The keypress event should only be fired
 *    when the corresponding key is for a printable character.
 *  - If a key requires a modifier key (e.g. "!" on a standard US keyboard), the sequence is: modifier down,
 *    key down, key up, modifier up, where key is the ideal unmodified key value (using the previous example, a "1").
 *  - Modifier keys (Ctrl, Shift, Alt, and Command/Meta) are assumed to be "sticky"; each modifier should be held down
 *    (e.g. only a keydown event) until either the modifier is encountered again in the sequence,
 *    or the NULL (U+E000) key is encountered.
 *  - Each key sequence is terminated with an implicit NULL key. Subsequently, all depressed modifier keys must
 *    be released (with corresponding keyup events) at the end of the sequence.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * JSON Parameters:
 *      value - {Array.<string>} The sequence of keys to type. An array must be provided.
 *              The server should flatten the array items to a single string to be typed.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 *      ElementNotVisible     - If the referenced element is not visible on the page
 *                              (either is hidden by CSS, has 0-width, or has 0-height)
 */
void WebDriverHub::elementValue(QString id, QStringList values)
{
    QJsonArray keys;

    if(values.size() > 0) {

        for(int i = 0; i < values.size(); i++) {
            keys.append(QString(values.at(i)));
        }
    }

    QJsonObject json;
    json["value"] = keys;

    post("element/" + id + "/value", json);
}

/**
 * POST /session/:sessionId/keys
 *
 * Send a sequence of key strokes to the active element. This command is similar to the send keys command in every
 * aspect except the implicit termination: The modifiers are not released at the end of the call.
 * Rather, the state of the modifier keys is kept between calls, so mouse interactions can be performed
 * while modifier keys are depressed.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      value - {Array.<string>} The keys sequence to be sent. The sequence is defined in thesend keys command.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 */
void WebDriverHub::keys(QString text)
{
    QJsonArray value;
    value.append(QString(text));

    QJsonObject json;
    json["value"] = value;

    post("keys", json);
}

/**
 * GET /session/:sessionId/element/:id/name
 *
 * Query for an element's tag name.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * Returns:
 *      {string} The element's tag name, as a lowercase string.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 */
QString WebDriverHub::getElementTagName(QString id)
{
    return getValueString("element/" + id + "/name");
}

/**
 * POST /session/:sessionId/element/:id/clear
 *
 * Clear a TEXTAREA or text INPUT element's value.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 *      ElementNotVisible     - If the referenced element is not visible on the page (either is hidden by CSS,
 *                              has 0-width, or has 0-height)
 *      InvalidElementState   - If the referenced element is disabled.
 */
void WebDriverHub::elementClear(QString id)
{
    post("element/" + id + "/clear");
}

/**
 * GET /session/:sessionId/element/:id/selected
 *
 * Determine if an OPTION element, or an INPUT element of type checkbox or radiobutton is currently selected.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * Returns:
 *      {boolean} Whether the element is selected.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 */
bool WebDriverHub::isElementSelected(QString id)
{
    return getValueBool("element/" + id + "/selected");
}

/**
 * GET /session/:sessionId/element/:id/enabled
 *
 * Determine if an element is currently enabled.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * Returns:
 *      {boolean} Whether the element is enabled.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 */
bool WebDriverHub::isElementEnabled(QString id)
{
    return getValueBool("element/" + id + "/enabled");
}

/**
 * GET /session/:sessionId/element/:id/attribute/:name
 *
 * Get the value of an element's attribute.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * Returns:
 *      {string|null} The value of the attribute, or null if it is not set on the element.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 */
QString WebDriverHub::getElementAttributeValue(QString id, QString name)
{
    return getValueString("element/" + id + "/attribute/" + name);
}

/**
 * GET /session/:sessionId/element/:id/equals/:other
 *
 * Test if two element IDs refer to the same DOM element.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *      :other     - ID of the element to compare against.
 *
 * Returns:
 *      {boolean} Whether the two IDs refer to the same element.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If either the element refered to by :id or :other is no longer attached to the page's DOM.
 */
bool WebDriverHub::elementEqualsOther(QString id, QString other)
{
    return getValueBool("element/" + id + "/equals/" + other);
}

/**
 * GET /session/:sessionId/element/:id/displayed
 *
 * Determine if an element is currently displayed.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * Returns:
 *      {boolean} Whether the element is displayed.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 */
bool WebDriverHub::isElementDisplayed(QString id)
{
    return getValueBool("element/" + id + "/displayed");
}

/**
 * GET /session/:sessionId/element/:id/location
 *
 * Determine an element's location on the page. The point (0, 0) refers to the upper-left corner of the page.
 * The element's coordinates are returned as a JSON object with x and y properties.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * Returns:
 *      {x:number, y:number} The X and Y coordinates for the element on the page.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 */
QPoint WebDriverHub::getElementLocation(QString id)
{
    QPoint location;
    location.setX(0);
    location.setY(0);

    QJsonObject result = getValueObject("element/" + id + "/location");

    if(result["x"].isDouble()) {
        location.setX(result["x"].toInt());
    } else {
        qDebug() << "Error get x localtion";
    }

    if(result["y"].isDouble()) {
        location.setY(result["y"].toInt());
    } else {
        qDebug() << "Error get y localtion";
    }

    return location;
}

/**
 * GET /session/:sessionId/element/:id/location_in_view
 *
 * Determine an element's location on the screen once it has been scrolled into view.
 *
 * Note: This is considered an internal command and should only be used to determine an element's location
 * for correctly generating native events.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * Returns:
 *      {x:number, y:number} The X and Y coordinates for the element.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 */
QPoint WebDriverHub::getElementLocationInView(QString id)
{
    QPoint location;
    location.setX(0);
    location.setY(0);

    QJsonObject result = getValueObject("element/" + id + "/location_in_view");

    if(result["x"].isDouble()) {
        location.setX(result["x"].toInt());
    } else {
        qDebug() << "Error get x localtion";
    }

    if(result["y"].isDouble()) {
        location.setY(result["y"].toInt());
    } else {
        qDebug() << "Error get y localtion";
    }

    return location;
}

/**
 * GET /session/:sessionId/element/:id/size
 *
 * Determine an element's size in pixels. The size will be returned as a JSON object with width and height properties.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * Returns:
 *      {width:number, height:number} The width and height of the element, in pixels.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 */
QSize WebDriverHub::getElementSize(QString id)
{
    QSize size;
    size.setWidth(0);
    size.setHeight(0);

    QJsonObject result = getValueObject("element/" + id + "/size");

    if(result["width"].isDouble()) {
        size.setWidth(result["width"].toInt());
    } else {
        qDebug()<<"Error get width value";
    }

    if(result["height"].isDouble()) {
        size.setWidth(result["height"].toInt());
    } else {
        qDebug()<<"Error get height value";
    }

    return size;
}

/**
 * GET /session/:sessionId/element/:id/css/:propertyName
 *
 * Query the value of an element's computed CSS property. The CSS property to query should be specified using the
 * CSS property name, not the JavaScript property name (e.g. background-color instead of backgroundColor).
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *      :id        - ID of the element to route the command to.
 *
 * Returns:
 *      {string} The value of the specified CSS property.
 *
 * Potential Errors:
 *      NoSuchWindow          - If the currently selected window has been closed.
 *      StaleElementReference - If the element referenced by :id is no longer attached to the page's DOM.
 */
QString WebDriverHub::getElementCssPropertyValue(QString id, QString propertyName)
{
    return getValueString("element/" + id + "/css/" + propertyName);
}

/**
 * GET /session/:sessionId/orientation
 *
 * Get the current browser orientation. The server should return a valid orientation value as defined
 * in ScreenOrientation: {LANDSCAPE|PORTRAIT}.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {string} The current browser orientation corresponding to a value defined
 *               in ScreenOrientation: {LANDSCAPE|PORTRAIT}.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 */
QString WebDriverHub::getOrientation()
{
    return getValueString("orientation");
}

/**
 * POST /session/:sessionId/orientation
 *
 * Set the browser orientation. The orientation should be specified as defined
 * in ScreenOrientation: {LANDSCAPE|PORTRAIT}.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      orientation - {string} The new browser orientation as defined in ScreenOrientation: {LANDSCAPE|PORTRAIT}.
 *
 * Potential Errors:
 *      NoSuchWindow - If the currently selected window has been closed.
 */
void WebDriverHub::setOrientation(QString orientation)
{
    QJsonObject json;
    json["orientation"] = orientation;

    post("orientation", json);
}

/**
 * GET /session/:sessionId/alert_text
 *
 * Gets the text of the currently displayed JavaScript alert(), confirm(), or prompt() dialog.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {string} The text of the currently displayed alert.
 *
 * Potential Errors:
 *      NoAlertPresent - If there is no alert displayed.
 */
QString WebDriverHub::getAlertText()
{
    return getValueString("alert_text");
}

/**
 * POST /session/:sessionId/alert_text
 *
 * Sends keystrokes to a JavaScript prompt() dialog.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      text - {string} Keystrokes to send to the prompt() dialog.
 *
 * Potential Errors:
 *      NoAlertPresent - If there is no alert displayed.
 */
void WebDriverHub::setAlertText(QString text)
{
    QJsonObject json;
    json["text"] = text;

    post("alert_text", json);
}

/**
 * POST /session/:sessionId/accept_alert
 *
 * Accepts the currently displayed alert dialog. Usually, this is equivalent to clicking on the 'OK' button in the dialog.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Potential Errors:
 *      NoAlertPresent - If there is no alert displayed.
 */
void WebDriverHub::acceptAlert()
{
    post("accept_alert");
}

/**
 * POST /session/:sessionId/dismiss_alert
 *
 * Dismisses the currently displayed alert dialog. For confirm() and prompt() dialogs, this is equivalent
 * to clicking the 'Cancel' button. For alert() dialogs, this is equivalent to clicking the 'OK' button.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Potential Errors:
 *      NoAlertPresent - If there is no alert displayed.
 */
void WebDriverHub::dismissAlert()
{
    post("dismiss_alert");
}

/**
 * POST /session/:sessionId/moveto
 *
 * Move the mouse by an offset of the specificed element. If no element is specified, the move is relative
 * to the current mouse cursor. If an element is provided but no offset, the mouse will be moved to the center
 * of the element. If the element is not visible, it will be scrolled into view.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * JSON Parameters:
 *      element - {string} Opaque ID assigned to the element to move to, as described in the WebElement JSON Object.
 *                If not specified or is null, the offset is relative to current position of the mouse.
 *      xoffset - {number} X offset to move to, relative to the top-left corner of the element.
 *                If not specified, the mouse will move to the middle of the element.
 *      yoffset - {number} Y offset to move to, relative to the top-left corner of the element.
 *                If not specified, the mouse will move to the middle of the element.
 */
void WebDriverHub::mouseMoveTo(QString element)
{
    QJsonObject json;
    json["element"] = element;

    post("moveto", json);
}

void WebDriverHub::mouseMoveTo(int xoffset, int yoffset)
{
    QJsonObject json;
    json["xoffset"] = xoffset;
    json["yoffset"] = yoffset;

    post("moveto", json);
}

/**
 * POST /session/:sessionId/click
 *
 * Click any mouse button (at the coordinates set by the last moveto command).
 * Note that calling this command after calling buttondown and before calling button up
 * (or any out­of­order interactions sequence) will yield undefined behaviour).
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * JSON Parameters:
 *      button ­- {number} Which button, enum: {LEFT = 0, MIDDLE = 1 , RIGHT = 2} .
 *               Defaults to the left mouse button if not specified.
 */
void WebDriverHub::mouseClick(int button)
{
    QJsonObject json;
    json["button"]  = button;

    post("click", json);
}

/**
 * POST /session/:sessionId/buttondown
 *
 * Click and hold the left mouse button (at the coordinates set by the last moveto command).
 * Note that the next mouse­related command that should follow is buttonup. Any other mouse command
 * (such as click or another call to buttondown) will yield undefined behaviour.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * JSON Parameters:
 *      button ­- {number} Which button, enum: {LEFT = 0, MIDDLE = 1 , RIGHT = 2}.
 *               Defaults to the left mouse button if not specified.
 */
void WebDriverHub::mouseButtonDown(int button)
{
    QJsonObject json;
    json["button"]  = button;

    post("buttondown", json);
}

/**
 * POST /session/:sessionId/buttonup
 *
 * Releases the mouse button previously held (where the mouse is currently at).
 * Must be called once for every buttondown command issued. See the note in click and buttondown about
 * implications of out­of­order commands.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * JSON Parameters:
 *      button ­- {number} Which button, enum: {LEFT = 0, MIDDLE = 1 , RIGHT = 2}.
 *               Defaults to the left mouse button if not specified.
 */
void WebDriverHub::mouseButtonUp(int button)
{
    QJsonObject json;
    json["button"]  = button;

    post("buttonup", json);
}

/**
 * POST /session/:sessionId/doubleclick
 *
 * Double­clicks at the current mouse coordinates (set by moveto).
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 */
void WebDriverHub::mouseDoubleClick()
{
    post("doubleclick");
}

/**
 * POST /session/:sessionId/touch/click
 *
 * Single tap on the touch enabled device.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * JSON Parameters:
 *      element ­- {string} ID of the element to single tap on.
 */
void WebDriverHub::touchClick(QString element)
{
    QJsonObject json;
    json["element"] = element;

    post("touch/click", json);
}

/**
 * POST /session/:sessionId/touch/down
 *
 * Finger down on the screen.
 *
 * URL Parameters:
 *      :sessionId ­- ID of the session to route the command to.
 *
 * JSON Parameters:
 *      x -­ {number} X coordinate on the screen.
 *      y - {number} Y coordinate on the screen.
 */
void WebDriverHub::touchDown(int x, int y)
{
    QJsonObject json;
    json["x"] = x;
    json["y"] = y;

    post("touch/down", json);
}

/**
 * POST /session/:sessionId/touch/up
 *
 * Finger up on the screen.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * JSON Parameters:
 *      x ­- {number} X coordinate on the screen.
 *      y -­ {number} Y coordinate on the screen.
 */
void WebDriverHub::touchUp(int x, int y)
{
    QJsonObject json;
    json["x"] = x;
    json["y"] = y;

    post("touch/up", json);
}

/**
 * POST session/:sessionId/touch/move
 *
 * Finger move on the screen.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * JSON Parameters:
 *      x ­- {number} X coordinate on the screen.
 *      y -­ {number} Y coordinate on the screen.
 */
void WebDriverHub::touchMove(int x, int y)
{
    QJsonObject json;
    json["x"] = x;
    json["y"] = y;

    post("touch/move", json);
}

/**
 * POST session/:sessionId/touch/scroll
 *
 * Scroll on the touch screen using finger based motion events.
 * Use this command to start scrolling at a particular screen location.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * JSON Parameters:
 *      element -­ {string} ID of the element where the scroll starts.
 *      xoffset ­- {number} The x offset in pixels to scroll by.
 *      yoffset -­ {number} The y offset in pixels to scroll by.
 */
void WebDriverHub::touchScroll(QString element, int xoffset, int yoffset)
{
    QJsonObject json;
    json["element"] = element;
    json["xoffset"] = xoffset;
    json["yoffset"] = yoffset;

    post("touch/scroll", json);
}

/**
 * POST session/:sessionId/touch/scroll
 *
 * Scroll on the touch screen using finger based motion events.
 * Use this command if you don't care where the scroll starts on the screen.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * JSON Parameters:
 *      xoffset -­ {number} The x offset in pixels to scrollby.
 *      yoffset -­ {number} The y offset in pixels to scrollby.
 */
void WebDriverHub::touchScroll(int xoffset, int yoffset)
{
    QJsonObject json;
    json["xoffset"] = xoffset;
    json["yoffset"] = yoffset;

    post("touch/scroll", json);
}

/**
 * POST session/:sessionId/touch/doubleclick
 *
 * Double tap on the touch screen using finger motion events.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * JSON Parameters:
 *      element -­ {string} ID of the element to double tap on.
 */
void WebDriverHub::touchDoubleClick(QString element)
{
    QJsonObject json;
    json["element"] = element;

    post("touch/doubleclick", json);
}

/**
 * POST session/:sessionId/touch/longclick
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * JSON Parameters:
 *      element -­ {string} ID of the element to long press on.

 */
void WebDriverHub::touchLongClick(QString element)
{
    QJsonObject json;
    json["element"] = element;

    post("touch/longclick", json);
}

/**
 * POST session/:sessionId/touch/flick
 *
 * Flick on the touch screen using finger motion events. This flickcommand starts at a particulat screen location.
 *
 * URL Parameters:
 *      :sessionId ­- ID of the session to route the command to.
 *
 * JSON Parameters:
 *      element -­ {string} ID of the element where the flick starts.
 *      xoffset -­ {number} The x offset in pixels to flick by.
 *      yoffset -­ {number} The y offset in pixels to flick by.
 *      speed   -­ {number} The speed in pixels per seconds.
 */
void WebDriverHub::touchFlick(QString element, int xoffset, int yoffset, int speed)
{
    QJsonObject json;
    json["element"] = element;
    json["xoffset"] = xoffset;
    json["yoffset"] = yoffset;
    json["speed"]   = speed;

    post("touch/flick", json);
}

/**
 * POST session/:sessionId/touch/flick
 *
 * Flick on the touch screen using finger motion events.
 * Use this flick command if you don't care where the flick starts on the screen.
 *
 * URL Parameters:
 *      :sessionId ­- ID of the session to route the command to.
 *
 * JSON Parameters:
 *      xspeed -­ {number} The x speed in pixels per second.
 *      yspeed ­- {number} The y speed in pixels per second.
 */
void WebDriverHub::touchFlick(int xspeed, int yspeed)
{
    QJsonObject json;
    json["xspeed"] = xspeed;
    json["yspeed"] = yspeed;

    post("touch/flick", json);
}

/**
 * GET /session/:sessionId/location
 *
 * Get the current geo location.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * Returns:
 *      {latitude: number, longitude: number, altitude: number} The current geo location.
 */
QJsonObject WebDriverHub::getGeoLocation()
{
    return getValueObject("location");
}

/**
 * POST /session/:sessionId/location
 *
 * Set the current geo location.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * JSON Parameters:
 *      location -­ {latitude: number, longitude: number, altitude: number} The new location.
 */
void WebDriverHub::setGeoLocation(int latitude, int longtude, int alttude)
{
    QJsonObject json;
    json["latitude"] = latitude;
    json["longtude"] = longtude;
    json["alttude"]  = alttude;

    post("location", json);
}

/**
 * GET /session/:sessionId/local_storage
 *
 * Get all keys of the storage.
 *
 * URL Parameters:
 *      :sessionId ­- ID of the session to route the command to.
 *
 * Returns:
 *      {Array.<string>} The list of keys.
 *
 * Potential Errors:
 *      NoSuchWindow -­ If the currently selected window has been closed.
 */
QStringList WebDriverHub::getAllKeysLocalStorage()
{
    return getValueArrayString("local_storage");
}

/**
 * POST /session/:sessionId/local_storage
 *
 * Set the storage item for the given key.
 *
 * URL Parameters:
 *      :sessionId ­ ID of the session to route the command to.
 *
 * JSON Parameters:
 *      key   -­ {string} The key to set.
 *      value -­ {string} The value to set.
 *
 * Potential Errors:
 *      NoSuchWindow ­- If the currently selected window has been closed.

 */
void WebDriverHub::setLocalStorageByKey(QString key, QString value)
{
    QJsonObject json;
    json["key"]   = key;
    json["value"] = value;

    post("local_storage", json);
}

/**
 * DELETE /session/:sessionId/local_storage
 *
 * Clear the storage.
 *
 * URL Parameters:
 *      :sessionId ­ ID of the session to route the command to.
 *
 * Potential Errors:
 *      NoSuchWindow ­- If the currently selected window has been closed.
 */
void WebDriverHub::deleteLocalStorage()
{
    deleteMethod("local_storage");
}

/**
 * GET /session/:sessionId/local_storage/key/:key
 *
 * Get the storage item for the given key.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *      :key       ­- The key to get.
 *
 * Potential Errors:
 *      NoSuchWindow -­ If the currently selected window has been closed.

 */
QString WebDriverHub::getLocalStorageValueByKey(QString key)
{
    return getValueString("local_storage/key/" + key);
}

/**
 * DELETE /session/:sessionId/local_storage/key/:key
 *
 * Remove the storage item for the given key.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *      :key       -­ The key to remove.
 *
 * Potential Errors:
 *      NoSuchWindow -­ If the currently selected window has been closed.
 */
void WebDriverHub::deleteLocalStorageByKey(QString key)
{
    deleteMethod("local_storage/key/" + key);
}

/**
 * GET /session/:sessionId/local_storage/size
 *
 * Get the number of items in the storage.
 *
 * URL Parameters:
 *      :sessionId - ID of the session to route the command to.
 *
 * Returns:
 *      {number} -The number of items in the storage.
 *
 * Potential Errors:
 *      NoSuchWindow ­- If the currently selected window has been closed.
 */
int WebDriverHub::getLocalStorageSize()
{
    return getValueInt("local_storage/size");
}

/**
 * GET /session/:sessionId/session_storage
 *
 * Get all keys of the storage.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * Returns:
 *      {Array.<string>} The list of keys.
 *
 * Potential Errors:
 *      NoSuchWindow -­ If the currently selected window has been closed.
 */
QStringList WebDriverHub::getAllKeysSessionStorage()
{
    return getValueArrayString("session_storage");
}

/**
 * POST /session/:sessionId/session_storage
 *
 * Set the storage item for the given key.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * JSON Parameters:
 *      key   -­ {string} The key to set.
 *      value -­ {string} The value to set.
 *
 * Potential Errors:
 *      NoSuchWindow -­ If the currently selected window has been closed.
 */
void WebDriverHub::setSessionStorageByKey(QString key, QString value)
{
    QJsonObject json;
    json["key"]   = key;
    json["value"] = value;

    post("session_storage", json);
}

/**
 * DELETE /session/:sessionId/session_storage
 *
 * Clear the storage.
 *
 * URL Parameters:
 *      :sessionId ­- ID of the session to route the command to.
 *
 * Potential Errors:
 *      NoSuchWindow -­ If the currently selected window has been closed.
 */
void WebDriverHub::deleteSessionStorage()
{
    deleteMethod("session_storage");
}

/**
 * GET /session/:sessionId/session_storage/key/:key
 *
 * Get the storage item for the given key.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *      :key       -­ The key to get.
 *
 * Potential Errors:
 *      NoSuchWindow ­- If the currently selected window has been closed.
 */
QString WebDriverHub::getSessionStorageValueByKey(QString key)
{
    return getValueString("session_storage/key/" + key);
}

/**
 * DELETE /session/:sessionId/session_storage/key/:key
 *
 * Remove the storage item for the given key.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *      :key       -­ The key to remove.
 *
 * Potential Errors:
 *      NoSuchWindow -­ If the currently selected window has been closed.
 */
void WebDriverHub::deleteSessionStorageKey(QString key)
{
    deleteMethod("session_storage/key/" + key);
}

/**
 * GET /session/:sessionId/session_storage/size
 *
 * Get the number of items in the storage.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * Returns:
 *      {number} The number of items in the storage.
 *
 * Potential Errors:
 *      NoSuchWindow -­ If the currently selected window has been closed.
 */
int WebDriverHub::getSessionStorageSize()
{
    return getValueInt("session_storage/size");
}

/**
 * POST /session/:sessionId/log
 *
 * Get the log for a given log type. Log buffer is reset after each request.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * JSON Parameters:
 *      type -­ {string} The log type. This must be provided.
 *
 * Returns:
 *      {Array.<object>} The list of log entries.
 */
QList<QJsonObject> WebDriverHub::getLog(QString type)
{
    QJsonObject json;
    json["type"] = type;

    QJsonObject result = post("log", json);

    QList<QJsonObject> log;
    if(result["value"].isArray()) {

        QJsonArray arr = result["value"].toArray();

        for(int i = 0; i < arr.size(); i++) {
            log.push_back(arr.at(i).toObject());
        }
    }

    return log;
}

/**
 * GET /session/:sessionId/log/types
 *
 * Get available log types.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * Returns:
 *      {Array.<string>} - The list of available log types.
 */
QStringList WebDriverHub::getLogTypes()
{
    return getValueArrayString("log/types");
}

/**
 * GET /session/:sessionId/application_cache/status
 *
 * Get the status of the html5 application cache.
 *
 * URL Parameters:
 *      :sessionId -­ ID of the session to route the command to.
 *
 * Returns:
 *      {number} - Status code for application cache: {UNCACHED = 0, IDLE = 1, CHECKING = 2, DOWNLOADING = 3,
 *                 UPDATE_READY = 4, OBSOLETE = 5}
 */
int WebDriverHub::getApplicationCacheStatus()
{
    return getValueInt("application_cache_status");
}
