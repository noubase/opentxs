/************************************************************
 *
 *                 OPEN TRANSACTIONS
 *
 *       Financial Cryptography and Digital Cash
 *       Library, Protocol, API, Server, CLI, GUI
 *
 *       -- Anonymous Numbered Accounts.
 *       -- Untraceable Digital Cash.
 *       -- Triple-Signed Receipts.
 *       -- Cheques, Vouchers, Transfers, Inboxes.
 *       -- Basket Currencies, Markets, Payment Plans.
 *       -- Signed, XML, Ricardian-style Contracts.
 *       -- Scripted smart contracts.
 *
 *  EMAIL:
 *  fellowtraveler@opentransactions.org
 *
 *  WEBSITE:
 *  http://www.opentransactions.org/
 *
 *  -----------------------------------------------------
 *
 *   LICENSE:
 *   This Source Code Form is subject to the terms of the
 *   Mozilla Public License, v. 2.0. If a copy of the MPL
 *   was not distributed with this file, You can obtain one
 *   at http://mozilla.org/MPL/2.0/.
 *
 *   DISCLAIMER:
 *   This program is distributed in the hope that it will
 *   be useful, but WITHOUT ANY WARRANTY; without even the
 *   implied warranty of MERCHANTABILITY or FITNESS FOR A
 *   PARTICULAR PURPOSE.  See the Mozilla Public License
 *   for more details.
 *
 ************************************************************/

#ifndef OPENTXS_CORE_CRYPTO_CREDENTIAL_HPP
#define OPENTXS_CORE_CRYPTO_CREDENTIAL_HPP

#include "opentxs/core/OTData.hpp"
#include "opentxs/core/Proto.hpp"
#include "opentxs/core/String.hpp"
#include "opentxs/core/Types.hpp"
#include "opentxs/core/contract/Signable.hpp"
#include "opentxs/core/crypto/OTASCIIArmor.hpp"

#include <stdint.h>
#include <memory>
#include <string>

// A nym contains a list of credential sets.
// The whole purpose of a Nym is to be an identity, which can have
// master credentials.
//
// Each CredentialSet contains list of Credentials. One of the
// Credentials is a MasterCredential, and the rest are ChildCredentials
// signed by the MasterCredential.
//
// A Credential may contain keys, in which case it is a KeyCredential.
//
// Credentials without keys might be an interface to a hardware device
// or other kind of external encryption and authentication system.
//
// Non-key Credentials are not yet implemented.
//
// Each KeyCredential has 3 OTKeypairs: encryption, signing, and authentication.
// Each OTKeypair has 2 OTAsymmetricKeys (public and private.)
//
// A MasterCredential must be a KeyCredential, and is only used to sign
// ChildCredentials
//
// ChildCredentials are used for all other actions, and never sign other
// Credentials

namespace opentxs
{

class Contract;
class CredentialSet;
class Identifier;
class NymParameters;
class OTPassword;
class OTPasswordData;

typedef std::shared_ptr<proto::Credential> serializedCredential;

class Credential : public Signable
{
public:
    static String CredentialTypeToString(proto::CredentialType credentialType);
    static serializedCredential ExtractArmoredCredential(
        const String stringCredential);
    static serializedCredential ExtractArmoredCredential(
        const OTASCIIArmor armoredCredential);
    static Credential* CredentialFactory(
        CredentialSet& parent,
        const proto::Credential& serialized,
        const proto::KeyMode& mode,
        const proto::CredentialRole& role = proto::CREDROLE_ERROR);
    template<class C>
    static C* Create(
        CredentialSet& owner,
        const NymParameters& nymParameters)
    {
        C* credential = new C(owner, nymParameters);
        if (nullptr != credential) {
            credential->New(nymParameters);
            credential->Save();
        }
        return credential;
    }

private:
    typedef Signable ot_super;
    Credential() = delete;

    // Syntax (non cryptographic) validation
    bool isValid() const;
    // Returns the serialized form to prevent unnecessary serializations
    bool isValid(serializedCredential& credential) const;

    Identifier GetID() const override;
    std::string Name() const override { return String(id_).Get(); }
    bool VerifyMasterID() const;
    bool VerifyNymID() const;
    bool VerifySignedByMaster() const;

protected:
    proto::CredentialType type_ = proto::CREDTYPE_ERROR;
    proto::CredentialRole role_ = proto::CREDROLE_ERROR;
    proto::KeyMode mode_ = proto::KEYMODE_ERROR;
    CredentialSet* owner_backlink_ = nullptr; // Do not cleanup.
    String master_id_;
    String nym_id_;
    uint32_t version_ = 0;

    Credential(CredentialSet& owner, const proto::Credential& serializedCred);
    Credential(CredentialSet& owner, const NymParameters& nymParameters);

    bool AddMasterSignature();
    virtual bool New(const NymParameters& nymParameters);

public:
    const String& MasterID() const { return master_id_; }
    const String& NymID() const { return nym_id_; }
    const proto::CredentialRole& Role() const { return role_; }
    SerializedSignature MasterSignature() const;
    const proto::KeyMode& Mode() const { return mode_; }
    bool Private() const { return (proto::KEYMODE_PRIVATE == mode_); }
    SerializedSignature SelfSignature(
        CredentialModeFlag version = PUBLIC_VERSION) const;
    SerializedSignature SourceSignature() const;
    proto::CredentialType Type() const;

    std::string asString(const bool asPrivate = false) const;
    virtual serializedCredential asSerialized(
        SerializationModeFlag asPrivate,
        SerializationSignatureFlag asSigned) const;
    virtual bool hasCapability(const NymCapability& capability) const;
    virtual bool VerifyInternally() const;

    virtual void ReleaseSignatures(const bool onlyPrivate);
    virtual bool Save() const;
    OTData Serialize() const override;
    bool Validate() const override;

    virtual bool GetContactData(
        std::unique_ptr<proto::ContactData>& contactData) const;
    virtual bool GetVerificationSet(
        std::unique_ptr<proto::VerificationSet>& verificationSet) const;

    virtual bool Verify(
        const OTData& plaintext,
        const proto::Signature& sig,
        const proto::KeyRole key = proto::KEYROLE_SIGN) const;
    virtual bool Verify(const Credential& credential) const;
    virtual bool TransportKey(OTData& publicKey, OTPassword& privateKey) const;

    virtual ~Credential();
};

} // namespace opentxs

#endif // OPENTXS_CORE_CRYPTO_CREDENTIAL_HPP
