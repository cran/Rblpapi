/* Copyright 2012. Bloomberg Finance L.P.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:  The above
 * copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/** \file blpapi_identity.h */
/** \defgroup blpapi_identity Component blpapi_identity
\brief Provide access to the entitlements for a user.
\file blpapi_identity.h
\brief Provide access to the entitlements for a user.
*/

#ifndef INCLUDED_BLPAPI_IDENTITY
#define INCLUDED_BLPAPI_IDENTITY

/** \addtogroup blpapi
 * @{
 */
/** \addtogroup blpapi_identity
 * @{
 * <A NAME="purpose"></A>
 * <A NAME="1"> \par Purpose: </A>
 * Provide access to the entitlements for a user.
 * \par
 * \par
 * <A NAME="classes"></A>
 * <A NAME="2"> \par Classes: </A>
 * <table>
 * <tr>
 * <td>blpapi::Identity</td>
 * <td>Represents an identification of a user</td>
 * </tr>
 * </table>
 * \par
 * \par
 * <A NAME="description"></A>
 * <A NAME="3"> \par Description: </A>
 *  This component provides an identification of a user and
 * implements the access to the entitlements.
 */
/** @} */
/** @} */

#include <blpapi_defs.h>
#include <blpapi_exception.h>
#include <blpapi_service.h>
#include <blpapi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

BLPAPI_EXPORT
void blpapi_Identity_release(blpapi_Identity_t *handle);

BLPAPI_EXPORT
int blpapi_Identity_addRef(blpapi_Identity_t *handle);

BLPAPI_EXPORT
int blpapi_Identity_hasEntitlements(const blpapi_Identity_t *handle,
        const blpapi_Service_t *service,
        const blpapi_Element_t *eidElement,
        const int *entitlementIds,
        size_t numEntitlements,
        int *failedEntitlements,
        int *failedEntitlementsCount);

BLPAPI_EXPORT
int blpapi_Identity_isAuthorized(
        const blpapi_Identity_t *handle, const blpapi_Service_t *service);

BLPAPI_EXPORT
int blpapi_Identity_getSeatType(
        const blpapi_Identity_t *handle, int *seatType);

#ifdef __cplusplus
}

#include <utility>

/** \addtogroup blpapi
 * @{
 */
/** \addtogroup blpapi_identity
 * @{
 */

namespace BloombergLP {
namespace blpapi {

class Element;

/*!
 *     Provides access to the entitlements for a specific user.
 *
 *     An unauthorized Identity is created using
 *     Session::createIdentity(). Once a Identity has been created
 *     it can be authorized using
 *     Session::sendAuthorizationRequest(). The authorized Identity
 *     can then be queried or used in Session::subscribe() or
 *     Session::sendRequest() calls.
 *
 *     Once authorized a Identity has access to the entitlements of
 *     the user which it was validated for.
 *
 *     The Identity is a reference counted handle, copying it or
 *     assigning it does not duplicate the underlying entitlement
 *     data. Once the last Identity referring to the underlying
 *     entitlement data is destroyed that entitlement data is
 *     discarded and can only be re-established using
 *     Session::sendAuthorizationRequest() again.
 */
/*!
 * See \ref blpapi_identity
 */
class Identity {

    blpapi_Identity_t *d_handle_p;

    void addRef();
    void release();

  public:
    enum SeatType {
        INVALID_SEAT = BLPAPI_SEATTYPE_INVALID_SEAT,
        ///< Invalid seat type. Also used for Identity instances that have not
        /// yet been authorized, or failed to authorize.

        BPS = BLPAPI_SEATTYPE_BPS,
        ///< Bloomberg Professional %Service

        NONBPS = BLPAPI_SEATTYPE_NONBPS
        ///< Not permissioned for Bloomberg Professional %Service
    };

  public:
    explicit Identity(blpapi_Identity_t *handle);
    /*!<
     *     Assume ownership of the raw handle
     */

    Identity();
    /*!<
     *     Create an uninitialized Identity. The only valid operations
     *     on an uninitialized Identity are assignment, isValid() and
     *     destruction.
     */

    Identity(const Identity& original);
    /*!<
     *     Copy constructor
     */

    ~Identity();
    /*!<
     *     Destructor. Destroying the last Identity for a specific
     *     user cancels any authorizations associated with it.
     */

    // MANIPULATORS

    Identity& operator=(const Identity&);
    /*!<
     *     Assignment operator.
     */

    // ACCESSORS
    bool hasEntitlements(const Service& service,
            const int *entitlementIds,
            size_t numEntitlements) const;
    /*!<
     * Return true if this <code>Identity</code> is authorized for the
     * specified <code>service</code> and the first
     * <code>numEntitlements</code> elements of the specified
     * <code>entitlementIds</code> array; otherwise return false. The behavior
     * is undefined unless <code>entitlementIds</code> is an array containing
     * at least <code>numEntitlements</code> elements.
     */

    bool hasEntitlements(const Service& service,
            const int *entitlementIds,
            size_t numEntitlements,
            int *failedEntitlements,
            int *failedEntitlementsCount) const;
    /*!<
     * Return <code>true</code> if this <code>Identity</code> is authorized
     * both for the specified <code>service</code> and for the first
     * <code>numEntitlements</code> elements in the specified
     * <code>entitlementIds</code> array; otherwise fill the specified
     * <code>failedEntitlements</code> array with the subset of
     * <code>entitlementIds</code> this <code>Identity</code> is not authorized
     * for, load the number of such entitlements into the specified
     * <code>failedEntitlementsCount</code>, and return <code>false</code>.
     *
     * <code>failedEntitlementsCount</code> is used both as an input and as an
     * output parameter:
     * - As input, it is the size of the specified
     *   <code>failedEntitlements</code> array, the function will fill only up
     *   to <code>*failedEntitlementsCount</code> elements.
     * - As output, it is the number of the failed entitlements in
     *   <code>failedEntitlements</code> array.
     */

    bool hasEntitlements(const Service& service,
            const Element& entitlementIds,
            int *failedEntitlements,
            int *failedEntitlementsCount) const;
    /*!<
     * Return <code>true</code> if this <code>Identity</code> is authorized for
     * the specified <code>service</code> and for each of the entitlement IDs
     * contained in the <code>entitlementIds</code>, which must be an
     * <code>Element</code> which is either an integer, or an array of
     * integers; otherwise, fill the specified <code>failedEntitlements</code>
     * array with the subset of entitlement IDs this <code>Identity</code> is
     * not authorized for, load the number of such entitlements into the
     * specified <code>failedEntitlementsCount</code>, and return
     * <code>false</code>.
     *
     * <code>failedEntitlementsCount</code> is used both as an input and as an
     * output parameter:
     * - As input, it is the size of the specified
     *   <code>failedEntitlements</code> array, the function will fill only up
     *   to <code>*failedEntitlementsCount</code> elements.
     * - As output, it is the number of the failed entitlements in
     *   <code>failedEntitlements</code> array.
     */

    bool isValid() const;
    /*!<
     * Return true if this <code>Identity</code> is valid; otherwise return
     * false. Note that a valid <code>Identity</code> has not necessarily been
     * authorized. This function is deprecated.
     */

    bool isAuthorized(const Service& service) const;
    /*!<
     * Return true if this <code>Identity</code> is authorized to consume the
     * specified <code>service</code>; otherwise return false.
     */

    SeatType getSeatType() const;
    /*!<
     * Return the seat type of this <code>Identity</code>.
     */

    blpapi_Identity_t *handle() const;
};

/** @} */
/** @} */

//=============================================================================
//                           INLINE FUNCTION DEFINITIONS
//=============================================================================

// --------------
// class Identity
// --------------

inline Identity::Identity()
    : d_handle_p(0)
{
}

inline Identity::Identity(blpapi_Identity_t *newHandle)
    : d_handle_p(newHandle)
{
}

inline Identity::Identity(const Identity& original)
    : d_handle_p(original.d_handle_p)
{
    addRef();
}

inline Identity::~Identity() { release(); }

inline Identity& Identity::operator=(const Identity& rhs)
{
    using std::swap;

    Identity tmp(rhs);
    swap(tmp.d_handle_p, d_handle_p);

    return *this;
}

inline void Identity::addRef()
{
    if (d_handle_p) {
        blpapi_Identity_addRef(d_handle_p);
    }
}

inline void Identity::release()
{
    if (d_handle_p) {
        blpapi_Identity_release(d_handle_p);
    }
}

inline bool Identity::hasEntitlements(const Service& service,
        const int *entitlementIds,
        size_t numEntitlements) const
{
    return blpapi_Identity_hasEntitlements(d_handle_p,
                   service.handle(),
                   0,
                   entitlementIds,
                   numEntitlements,
                   0,
                   0)
            ? true
            : false;
}

inline bool Identity::hasEntitlements(const Service& service,
        const int *entitlementIds,
        size_t numEntitlements,
        int *failedEntitlements,
        int *failedEntitlementsCount) const
{
    return blpapi_Identity_hasEntitlements(d_handle_p,
                   service.handle(),
                   0,
                   entitlementIds,
                   numEntitlements,
                   failedEntitlements,
                   failedEntitlementsCount)
            ? true
            : false;
}

inline bool Identity::hasEntitlements(const Service& service,
        const Element& entitlementIds,
        int *failedEntitlements,
        int *failedEntitlementsCount) const
{
    return blpapi_Identity_hasEntitlements(d_handle_p,
                   service.handle(),
                   entitlementIds.handle(),
                   0,
                   0,
                   failedEntitlements,
                   failedEntitlementsCount)
            ? true
            : false;
}

inline bool Identity::isValid() const { return (d_handle_p != 0); }

inline bool Identity::isAuthorized(const Service& service) const
{
    return blpapi_Identity_isAuthorized(d_handle_p, service.handle()) ? true
                                                                      : false;
}

inline Identity::SeatType Identity::getSeatType() const
{
    int seatType = BLPAPI_SEATTYPE_INVALID_SEAT;
    ExceptionUtil::throwOnError(
            blpapi_Identity_getSeatType(d_handle_p, &seatType));
    return static_cast<SeatType>(seatType);
}

inline blpapi_Identity_t *Identity::handle() const { return d_handle_p; }

} // close namespace blpapi
} // close namespace BloombergLP

#endif // #ifdef __cplusplus
#endif // #ifndef INCLUDED_BLPAPI_IDENTITY
