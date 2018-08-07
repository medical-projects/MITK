/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef MITKUSCombinedModality_H_HEADER_INCLUDED_
#define MITKUSCombinedModality_H_HEADER_INCLUDED_

#include <MitkUSNavigationExports.h>
#include "mitkUSDevice.h"
#include "mitkImageSource.h"
#include "mitkAbstractUltrasoundTrackerDevice.h"
#include "mitkNavigationDataSource.h"

namespace itk {
  template<class T> class SmartPointer;
}

namespace mitk {
  class USControlInterfaceBMode;
  class USControlInterfaceProbes;
  class USControlInterfaceDoppler;

  /**
   * \brief Combination of USDevice and NavigationDataSource.
   * This class can be used as an ImageSource subclass. Additionally tracking data be
   * retrieved from the NavigationDataSource returned by GetTrackingDevice().
   *
   * A calibration of the ultrasound image stream to the navigation datas can be set
   * for the currently active zoom level (of the ultrasound device) by SetCalibration().
   * The ultrasound images are transformed according to this calibration in the
   * GenerateData() method.
   */
  class MITKUSNAVIGATION_EXPORT USCombinedModality : public mitk::AbstractUltrasoundTrackerDevice
  {
  public:

    mitkClassMacro(USCombinedModality, mitk::AbstractUltrasoundTrackerDevice);
    mitkNewMacro3Param(USCombinedModality, USDevice::Pointer, itk::SmartPointer<NavigationDataSource>, bool);

    itkGetMacro(UltrasoundDevice, itk::SmartPointer<USDevice>);
    itkSetMacro(UltrasoundDevice, itk::SmartPointer<USDevice>);
    itkGetMacro(TrackingDevice, itk::SmartPointer<NavigationDataSource>);
    itkSetMacro(TrackingDevice, itk::SmartPointer<NavigationDataSource>);

    /**
    * \brief Getter for calibration data of the currently active depth and probe.
    *
    * \return Transformation for calibration or null if no calibration is available.
    */
    AffineTransform3D::Pointer GetCalibration();

    /**
     * \brief Getter for calibration data of the given depth and the currently active probe.
     *
     * \param depth depth of the b mode ultrasound image for which the calibration should be returned
     * \return Transformation for calibration or null if no calibration is available.
     */
    AffineTransform3D::Pointer GetCalibration(std::string depth);

    /**
     * \brief Getter for calibration data of the given depth and probe.
     *
     * \param depth depth of the b mode ultrasound image for which the calibration should be returned
     * \param probe probe of the ultrasound device for which the calibration should be returned
     * \return Transformation for calibration or null if no calibration is available.
     */
    AffineTransform3D::Pointer GetCalibration(std::string depth, std::string probe);

    /**
    * \brief Sets a transformation as calibration data.
    * Calibration data is set for the currently activated probe and their current
    * zoom factor. It also marks the device as calibrated.
    */
    void SetCalibration(AffineTransform3D::Pointer calibration);

    /**
     * \brief Removes the calibration data of the currently active depth and probe.
     * \return true on success, false if there was no calibration
     */
    bool RemoveCalibration();

    /**
     * \brief Removes the calibration data of the given depth and the currently active probe.
     *
     * \param depth depth of the b mode ultrasound image for which the calibration should be removed
     * \return true on success, false if there was no calibration
     */
    bool RemoveCalibration(std::string depth);

    /**
     * \brief Removes the calibration data of the given depth and probe.
     *
     * \param depth depth of the b mode ultrasound image for which the calibration should be removed
     * \param probe probe of the ultrasound device for which the calibration should be removed
     * \return true on success, false if there was no calibration
     */
    bool RemoveCalibration(std::string depth, std::string probe);

    /**
    * \brief Returns the Class of the Device.
    */
    std::string GetDeviceClass() override;

    /**
    * \brief Wrapper for returning USImageSource of the UltrasoundDevice.
    */
    USImageSource::Pointer GetUSImageSource() override;

    /**
    * \brief Wrapper for returning custom control interface of the UltrasoundDevice.
    */
    itk::SmartPointer<USAbstractControlInterface> GetControlInterfaceCustom() override;

    /**
    * \brief Wrapper for returning B mode control interface of the UltrasoundDevice.
    */
    itk::SmartPointer<USControlInterfaceBMode> GetControlInterfaceBMode() override;

    /**
    * \brief Wrapper for returning probes control interface of the UltrasoundDevice.
    */
    itk::SmartPointer<USControlInterfaceProbes> GetControlInterfaceProbes() override;

    /**
    * \brief Wrapper for returning doppler control interface of the UltrasoundDevice.
    */
    itk::SmartPointer<USControlInterfaceDoppler> GetControlInterfaceDoppler() override;

    virtual itk::SmartPointer<mitk::NavigationDataSource> GetNavigationDataSource();

    /**
     * \return true if the device is calibrated for the currently selected probe with the current zoom level
     */
    bool GetIsCalibratedForCurrentStatus();

    /**
     * \return true if a calibration was loaded for at least one probe and depth
     */
    bool GetContainsAtLeastOneCalibration();

    /**
    * \brief Remove this device from the micro service.
    * This method is public for mitk::USCombinedModality, because this devices
    * can be completly removed. This is not possible for API devices, which
    * should be available while their sub module is loaded.
    */
    void UnregisterOnService();

    /**
    * \brief Serializes all contained calibrations into an xml fragment.
    *
    * The returned string contains one parent node named "calibrations" and several
    * subnodes, one for each calibration that is present.
    */
    std::string SerializeCalibration();

    /**
    * \brief Deserializes a string provided by a prior call to Serialize().
    * If the bool flag is true, all prior calibrations will be deleted.
    * If the flag is set to false, prior calibrations will be retained, but overwritten
    * if one of equal name is present.
    *
    * \throws mitk::Exception if the given string could not be parsed correctly.
    */
    void DeserializeCalibration(const std::string &xmlString, bool clearPreviousCalibrations = true);

    void SetNumberOfSmoothingValues(unsigned int numberOfSmoothingValues);

    void SetDelayCount(unsigned int delayCount);

    void RegisterAsMicroservice();

    /**
    *\brief These Constants are used in conjunction with Microservices
    */
    static const std::string US_INTERFACE_NAME;
    static const std::string US_PROPKEY_DEVICENAME;
    static const std::string US_PROPKEY_CLASS;
    static const std::string US_PROPKEY_ID;
  protected:
    USCombinedModality( USDevice::Pointer usDevice,
                        itk::SmartPointer<NavigationDataSource> trackingDevice,
                        bool trackedUltrasoundActive = false );
    ~USCombinedModality() override;

    /**
    * \brief Initializes UltrasoundDevice.
    */
    bool OnInitialization() override;

    /**
    * \brief Connects UltrasoundDevice.
    */
    bool OnConnection() override;

    /**
    * \brief Disconnects UltrasoundDevice.
    */
    bool OnDisconnection() override;

    /**
    * \brief Activates UltrasoundDevice.
    */
    bool OnActivation() override;

    /**
    * \brief Deactivates UltrasoundDevice.
    */
    bool OnDeactivation() override;

    /**
    * \brief Freezes or unfreezes UltrasoundDevice.
    */
    void OnFreeze(bool) override;

    /**
    * \brief Grabs the next frame from the input.
    * This method is called internally, whenever Update() is invoked by an Output.
    */
    void GenerateData() override;

    std::string GetIdentifierForCurrentCalibration();
    std::string GetIdentifierForCurrentProbe();
    std::string GetCurrentDepthValue();

    void RebuildFilterPipeline();

    USDevice::Pointer                                      m_UltrasoundDevice;
    itk::SmartPointer<NavigationDataSource>                m_TrackingDevice;
    std::map<std::string, AffineTransform3D::Pointer>      m_Calibrations;

    itk::SmartPointer<mitk::NavigationDataSmoothingFilter> m_SmoothingFilter;
    itk::SmartPointer<mitk::NavigationDataDelayFilter>     m_DelayFilter;
    itk::SmartPointer<mitk::NavigationDataSource>          m_LastFilter;

    unsigned int m_NumberOfSmoothingValues;
    unsigned int m_DelayCount;

  };
} // namespace mitk

//MITK_DECLARE_SERVICE_INTERFACE(mitk::USCombinedModality, "org.mitk.services.USCombinedModality")
#endif // MITKUSCombinedModality_H_HEADER_INCLUDED_
