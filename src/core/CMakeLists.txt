# Copyright (c) Monetas AG, 2014

add_subdirectory(otprotob)

set(cxx-sources
  Timer.cpp
  tinythread.cpp
  anyoption.cpp
  mkcert.cpp
  OTAccount.cpp
  OTAcctFunctor.cpp
  OTAcctList.cpp
  OTAgent.cpp
  OTAgreement.cpp
  OTAmount.cpp
  OTASCIIArmor.cpp
  OTAssert.cpp
  OTAssetContract.cpp
  OTAsymmetricKey.cpp
  OTAsymmetricKeyOpenSSL.cpp
  OTAsymmetricKeyOpenSSLPrivdp.cpp
  OTBasket.cpp
  OTBasketItem.cpp
  OTBylaw.cpp
  OTCachedKey.cpp
  OTCallback.cpp
  OTCaller.cpp
  OTCheque.cpp
  OTClause.cpp
  OTContract.cpp
  OTCredential.cpp
  OTCron.cpp
  OTCronItem.cpp
  OTCrypto.cpp
  OTData.cpp
  OTDataCheck.cpp
  OTDataFolder.cpp
  OTDigitalCash.cpp
  OTEnvelope.cpp
  OTFolders.cpp
  OTIdentifier.cpp
  OTInstrument.cpp
  OTItem.cpp
  OTKeyCredential.cpp
  OTKeypair.cpp
  OTKeyring.cpp
  OTLedger.cpp
  OTLog.cpp
  OTLowLevelKeyData.cpp
  OTMarket.cpp
  OTMasterkey.cpp
  OTMessage.cpp
  OTMessageBuffer.cpp
  OTMint.cpp
  OTMintLucre.cpp
  OTNumList.cpp
  OTNymOrSymmetricKey.cpp
  OTOffer.cpp
  OTParty.cpp
  OTPartyAccount.cpp
  OTPassword.cpp
  OTPasswordData.cpp
  OTPaths.cpp
  OTPayload.cpp
  OTPayment.cpp
  OTPaymentPlan.cpp
  OTPseudonym.cpp
  OTPurse.cpp
  OTScript.cpp
  OTScriptable.cpp
  OTScriptChai.cpp
  OTServerContract.cpp
  OTSettings.cpp
  OTSignature.cpp
  OTSignedFile.cpp
  OTSmartContract.cpp
  OTStash.cpp
  OTStashItem.cpp
  OTStorage.cpp
  OTString.cpp
  OTStringXML.cpp
  OTSubcredential.cpp
  OTSubkey.cpp
  OTSymmetricKey.cpp
  OTToken.cpp
  OTTokenLucre.cpp
  OTTrackable.cpp
  OTTrade.cpp
  OTTransaction.cpp
  OTTransactionType.cpp
  OTVariable.cpp
  OTWallet.cpp
)

set(dependency_include_dir
  ${CMAKE_CURRENT_SOURCE_DIR}/../../deps/
)

include_directories(SYSTEM
  ${dependency_include_dir}
  ${dependency_include_dir}/bigint/
  ${dependency_include_dir}/ChaiScript/include/
  ${dependency_include_dir}/irrxml/
  ${dependency_include_dir}/lucre/include/
  ${dependency_include_dir}/simpleini/
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../include/opentxs/core
  ${CMAKE_BINARY_DIR}/src/core/otprotob
)

add_library(opentxs-core
  ${cxx-sources}
)

target_link_libraries(opentxs-core otprotob bigint irrxml lucre protobuf ssl crypto z ${OPENTXS_SYSTEM_LIBRARIES})