/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   TargomanAPI is free software: you can redistribute it and/or modify
#   it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   TargomanAPI is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU AFFERO GENERAL PUBLIC LICENSE for more details.
#
#   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 */

#include "FormalityChecker.h"
#include "libTargomanCommon/Configuration/Validators.hpp"

namespace Targoman {
namespace Apps {

using namespace Common;
using namespace Common::Configuration;

QString gConfigs::Classifier::makeConfig(const QString& _name)
{
    return "/Classifier/" + _name;
}

tmplConfigurable<double>     gConfigs::Classifier::Threshold(
        gConfigs::Classifier::makeConfig("Threshold"),
        "Threshold to be used by classifier",
        0.8,
        ReturnTrueCrossValidator(),
        "", "", "fasttext-threshold",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<FilePath_t>     gConfigs::Classifier::ModelPath(
        gConfigs::Classifier::makeConfig("ModelPath"),
        "Directory where FastText models are stored.",
        "",
        Validators::tmplPathAccessValidator<
        TARGOMAN_PATH_ACCESS(enuPathAccess::Dir | enuPathAccess::Readable),
        false>,
        "", "", "fasttext-models-path",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> gConfigs::Classifier::ModelPattern(
        gConfigs::Classifier::makeConfig("ModelPattern"),
        "File pattern for FastText models must have <%LANG%> pattern as language placeholder",
        "fasttext-%LANG%.bin",
        ReturnTrueCrossValidator(),
        "",
        "",
        "fasttext-model-pattrn",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<bool> gConfigs::Classifier::SupportsIXML(
        gConfigs::Classifier::makeConfig("SupportsIXML"),
        "Wheter the classifier was trained with IXML markups or not",
        true,
        ReturnTrueCrossValidator(),
        "", "", "fasttext-supports-ixml",
        enuConfigSource::Arg | enuConfigSource::File);

}
}
