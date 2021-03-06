/*
  Copyright (c) 2012 Vinícius dos Santos Oliveira

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
  */

#ifndef TUFAO_SESSIONSETTINGS_H
#define TUFAO_SESSIONSETTINGS_H

#include <QtNetwork/QNetworkCookie>
#include <QtCore/QDateTime>

namespace Tufao {

/*!
 * The SessionSettings class exposes details that sessions use to handle
 * cookies. Cookies are a mechanism to store state in the mostly stateless HTTP
 * protocol. These details are the cookies attributes.
 *
 * \note
 * Cookies don't provide isolation by port. For example, if a cookie is
 * accessible by a service running on one port, it will also be accessible by a
 * service running on another port on the same server.
 *
 * \par
 * \note
 * Cookies also don't provide isolation by scheme (HTTPS, HTTP, FTP, ...).
 *
 * \warning
 * You should *not* create SessionSetting objects with equal names and different
 * domain and paths hoping that SessionStore and some other objects making use
 * of SessionSettings will work correctly. When a user agent sends a cookie, the
 * only attributes sent in the request are the name and value pair, making
 * impossible, in several cases, to identify the right cookie.
 *
 * \since
 * 0.4
 */
struct SessionSettings
{
    /*!
     * Creates a cookie, using \p value as the cookie's value.
     */
    QNetworkCookie cookie(const QByteArray &value = QByteArray()) const
    {
        return cookie(*this, value);
    }

    /*!
     * Creates a cookie, using \p value as the cookie's value and \p settings
     * as cookie's settings.
     */
    static QNetworkCookie cookie(const SessionSettings &settings,
                                 const QByteArray &value = QByteArray())
    {
        QNetworkCookie cookie;

        if (settings.timeout) {
            cookie.setExpirationDate(QDateTime::currentDateTimeUtc()
                                     .addSecs(settings.timeout * 60));
        }
        cookie.setHttpOnly(settings.httpOnly);
        cookie.setSecure(settings.secure);
        if (!settings.domain.isEmpty()) cookie.setDomain(settings.domain);
        if (!settings.path.isEmpty()) cookie.setPath(settings.path);

        cookie.setName(settings.name);
        cookie.setValue(value);

        return cookie;
    }

    /*!
     * Define the lifetime of cookies generated by this object (a timeout
     * specified in minutes).
     *
     * The expiration date time is renewed every time SessionSetting generates a
     * cookie.
     *
     * \note
     * When not specified (the value is zero), the cookie expires at the end of
     * the user agent current session (as defined by the user agent).
     */
    int timeout;

    /*!
     * Whether cookies generated by this object should only be used in HTTP
     * requests. It prevents, for example, scripting engines in the user agent
     * from accessing the cookie.
     *
     * \note
     * You should turn this attribute true if you will use this cookie to
     * store sensitive data.
     */
    bool httpOnly;

    /*!
     * The name to which cookies generated by this object are used. This is the
     * main cookie access key.
     */
    QByteArray name;

    /*!
     * The set of paths to which cookies generated by this object are used.
     *
     * Let's name this value as cookiePath and the path component of the uri of
     * a request as requestPath. The cookie will be included in a request if one
     * of the following conditions is true:
     *   - cookiePath == requestPath
     *   - requestPath.startsWith(cookiePath)
     *   - requestPath[0] == '/' && requestPath.mid(1).startsWith(cookiePath)
     *
     * \note
     * If it's not specified, the user agent will choose a path based on the
     * current request's uri path component.
     *
     * \par
     * \note
     * Cookies don't provide integrity protection to this attribute. For
     * example, a service running on the path "/foo" can set a cookie with a
     * path attribute with the value "/bar". As a result, servers *should not*
     * both run mutually distrusting services on different paths of the same
     * host and use cookies to store sensitive data.
     */
    QByteArray path;

    /*!
     * Whether cookies generated by this object should *only* be used through
     * secure connections. What "secure" channels means are defined by the user
     * agent. This is typically HTTP over TLS.
     *
     * \note
     * This attribute only protects cookie's confidentiality. An active network
     * attacker can overwrite secure cookies from an insecure channel,
     * disrupting their integrity.
     *
     * \note
     * You should turn this attribute true if you will use this cookie to
     * store sensitive data.
     *
     * \sa
     * HttpsServer
     */
    bool secure;

    /*!
     * The hosts to which cookies generated by this object are used.
     *
     * \note
     * Subdomains are also considered. For example, if this value is
     * "example.com", the cookie generated by this SessionSettings object will
     * also be used when the user agent request some resource to the host
     * "wwww.example.com" and "www.corp.example.com".
     *
     * \par
     * \note
     * User agents will reject cookies unless this attribute specifies a scope
     * for the cookie that would include the origin server. For example, it will
     * accept the value "example.com" or "foo.example.com" coming from the
     * server "foo.example.com", but it will reject the value "bar.example.com".
     *
     * \note
     * For security reasons, many user agents are configured to reject values
     * that correspond to public suffixes, such as "com" and "co.uk".
     * 
     * \par
     * \note
     * If is not specified, the default behaviour, the user agent will only
     * include the cookie to requests made to the origin server. In other words,
     * it will, for example, exclude any subdomains.
     */
    QByteArray domain;
};

} // namespace Tufao

#endif // TUFAO_SESSION_H
