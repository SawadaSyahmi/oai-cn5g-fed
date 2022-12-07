/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "asn.1/Information Element Definitions.asn1"
 * 	`asn1c -pdu=all -fcompound-names -fno-include-deps -findirect-choice
 * -gen-PER -D src`
 */

#ifndef _Ngap_MICOModeIndication_H_
#define _Ngap_MICOModeIndication_H_

#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Ngap_MICOModeIndication {
  Ngap_MICOModeIndication_true = 0
  /*
   * Enumeration is extensible
   */
} e_Ngap_MICOModeIndication;

/* Ngap_MICOModeIndication */
typedef long Ngap_MICOModeIndication_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_Ngap_MICOModeIndication_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_Ngap_MICOModeIndication;
extern const asn_INTEGER_specifics_t asn_SPC_Ngap_MICOModeIndication_specs_1;
asn_struct_free_f Ngap_MICOModeIndication_free;
asn_struct_print_f Ngap_MICOModeIndication_print;
asn_constr_check_f Ngap_MICOModeIndication_constraint;
ber_type_decoder_f Ngap_MICOModeIndication_decode_ber;
der_type_encoder_f Ngap_MICOModeIndication_encode_der;
xer_type_decoder_f Ngap_MICOModeIndication_decode_xer;
xer_type_encoder_f Ngap_MICOModeIndication_encode_xer;
oer_type_decoder_f Ngap_MICOModeIndication_decode_oer;
oer_type_encoder_f Ngap_MICOModeIndication_encode_oer;
per_type_decoder_f Ngap_MICOModeIndication_decode_uper;
per_type_encoder_f Ngap_MICOModeIndication_encode_uper;
per_type_decoder_f Ngap_MICOModeIndication_decode_aper;
per_type_encoder_f Ngap_MICOModeIndication_encode_aper;

#ifdef __cplusplus
}
#endif

#endif /* _Ngap_MICOModeIndication_H_ */
#include <asn_internal.h>
